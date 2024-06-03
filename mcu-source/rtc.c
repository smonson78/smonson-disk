#include <string.h>

#include "stm32c0xx.h"
#include "spi.h"
#include "rtc.h"
#include "debug.h"
#include "timer0.h"

#include "stdutil.h"

void rtc_select() {
    CLOCK_PORT->BSRR = BSR_LOW(CLOCK_PIN);
}

void rtc_unselect() {
    CLOCK_PORT->BSRR = BSR_HIGH(CLOCK_PIN);
}

void rtc_setup() {
    // Setup SS pin as output
    CLOCK_PORT->MODER &= MODE_MASK(CLOCK_PIN);
    CLOCK_PORT->MODER |= MODE_OUTPUT(CLOCK_PIN);
    //CLOCK_PORT->OTYPER &= OTYPE_MASK(CLOCK_PIN);
    //CLOCK_PORT->OTYPER |= OTYPE_OPEN_DRAIN(CLOCK_PIN);

    CLOCK_PORT->OSPEEDR &= OSPEED_MASK(CLOCK_PIN);
    CLOCK_PORT->OSPEEDR |= OSPEED_VFAST(CLOCK_PIN);

    rtc_unselect();
}

// Write some bytes into the clock memory
void rtc_write(uint8_t addr, uint8_t *data, uint8_t len) {

    rtc_select();

    spi_transfer(MCP7951_WRITE);
    spi_transfer(addr); // address

    for (uint8_t i = 0; i < len; i++) {
        spi_transfer(data[i]);
    }
    
    rtc_unselect();
}

void rtc_read(uint8_t addr, uint8_t *dest, uint8_t len) {
    rtc_select();

    spi_transfer(MCP7951_READ);
    spi_transfer(addr);
    for (uint8_t i = 0; i < len; i++) {
        dest[i] = spi_transfer(0xff); 
    }
    
    rtc_unselect();
}

uint8_t getbcd(uint16_t num) {
    return ((num / 10) << 4) | (num % 10);
}

void rtc_set(datetime_t *datetime) {
    // TODO: 
    // 1. clear ST flag
    // 2. wait for OSCRUN flag to go to 0
    // 3. Load new values
    // 4. Set the ST flag again

    uint8_t rtc_buf[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    // Clear ST and VBAT flags
    // GCC thinks this leaves the memory uninitialised:
    //memset(rtc_buf, 0, 4);
    spi_slow();

    rtc_write(1, rtc_buf, 4); // Clears ST bit in addr 1 and VBATEN bit in addr 4
    debug("Wrote stop-osc command");

    // Wait for oscillator to stop running
    uint8_t success = 0;
    for (uint8_t i = 0; i < 255; i++) {
        rtc_read(4, &rtc_buf[0], 1);

        debug_nocr("Read ");
        debug_hex(rtc_buf[0], 2);
        debug(" from addr 4");

        if ((rtc_buf[0] & _BV(5)) == 0) {
            // Oscillator is not running.
            success = 1;
            break;
        }

        _delay_ms(10);
    }

    if (!success) {
        debug("Oscillator wouldn't stop");
        return;
    }

    // Minutes
    rtc_buf[2] = getbcd(datetime->minute) & 0x7f;
    // Hours
    rtc_buf[3] = getbcd(datetime->hour) & 0x7f;
    // Forget day-of-week, who cares
    rtc_buf[4] = 0;
    // Day
    rtc_buf[5] = getbcd(datetime->day) & 0x3f;
    // Month
    rtc_buf[6] = getbcd(datetime->month) & 0x1f;
    // TODO: Leap Year bit is also in byte 6
    // Year
    rtc_buf[7] = getbcd(datetime->year - 1980);
    // Seconds
    rtc_buf[1] = getbcd(datetime->second) & 0x7f;
    rtc_write(0, rtc_buf, 8);

    // Start oscillator, ST bit at address 1, bit 7
    rtc_buf[1] |= 0x80;
    rtc_write(1, rtc_buf + 1, 1);

    // Wait for oscillator to start up
    success = 0;
    debug("Starting osc");
    for (uint8_t i = 0; i < 64; i++) {
        rtc_read(4, &rtc_buf[0], 1);

        debug_nocr("Read ");
        debug_hex(rtc_buf[0], 2);
        debug(" from addr 4");        

        if (rtc_buf[0] & _BV(5)) {
            success = 1;
            break;
        }

        _delay_ms(40);
    }

    if (success) {
        // Enable battery backup, VBATEN bit at address 4, bit 3
        rtc_buf[4] |= _BV(3);
        rtc_write(4, rtc_buf + 4, 1);
    } else {
        debug("Oscillator never started up");
    }
}

void rtc_get(datetime_t *dest) {
    uint8_t rtc_buf[8];
    spi_slow();

    rtc_read(0, rtc_buf, 8);

    // Bloody BCD, why are they still using it in 2024
    //dest->hundredths = ((rtc_buf[0] >> 4) * 10) + (rtc_buf[0] & 0xf);
    uint8_t started = rtc_buf[1] >> 7;
    uint8_t oscrun = (rtc_buf[4] >> 5) & 1;
    uint8_t pwrfail = (rtc_buf[4] >> 4) & 1;
    uint8_t vbaten = (rtc_buf[4] >> 3) & 1;
    //uint8_t format = (rtc_buf[3] >> 6) & 1;
    //uint8_t ampm = (rtc_buf[3] >> 5) & 1;

    dest->second = (((rtc_buf[1] >> 4) & 7) * 10) + (rtc_buf[1] & 0xf);
    dest->minute = (((rtc_buf[2] >> 4) & 7) * 10) + (rtc_buf[2] & 0xf);
    dest->hour = (((rtc_buf[3] >> 4) & 3) * 10) + (rtc_buf[3] & 0xf);

    dest->day = (((rtc_buf[5] >> 4) & 3) * 10) + (rtc_buf[5] & 0xf);
    dest->month = (((rtc_buf[6] >> 4) & 1) * 10) + (rtc_buf[6] & 0xf);
    dest->year = (((rtc_buf[7] >> 4) & 0xf) * 10) + (rtc_buf[7] & 0xf);
    dest->year += 1980;

    debug("Read clock:");

    debug_nocr("Started: ");
    debug_decimal(started);
    debug("");

    debug_nocr("Oscillator running: ");
    debug_decimal(oscrun);
    debug("");

    debug_nocr("Power failure: ");
    debug_decimal(pwrfail);
    debug("");

    debug_nocr("Battery voltage status: ");
    debug_decimal(vbaten);
    debug("");

    // Show date as YY-MM-DD
    debug_decimal(dest->year);
    debug_nocr("-");
    
    debug_decimal(dest->month);
    debug_nocr("-");

    debug_decimal(dest->day);
    debug_nocr(" ");

    // Show time as HH:MM:SS.XX
    debug_decimal(dest->hour);
    debug_nocr(":");
    
    debug_decimal(dest->minute);
    debug_nocr(":");

    debug_decimal(dest->second);

    debug("");

}