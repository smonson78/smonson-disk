#include <avr/io.h>
#include <util/delay_basic.h>
#include <util/delay.h>

#include "spi.h"
#include "rtc.h"
#include "debug.h"

void rtc_select() {
    CLOCK_PORT.OUTCLR = CLOCK_PIN;
}

void rtc_unselect() {
    CLOCK_PORT.OUTSET = CLOCK_PIN;
}

void rtc_setup() {
    // Setup SS pin as output
    CLOCK_PORT.DIRSET = CLOCK_PIN;

    rtc_unselect();
}

// Write some bytes into the clock memory
void rtc_write(uint8_t addr, uint8_t *data, uint8_t len) {

    spi_slow();
    rtc_select();

    spi_transfer(MCP7951_WRITE);
    spi_transfer(addr); // address

    for (uint8_t i = 0; i < len; i++) {
        spi_transfer(data[i]);
    }
    
    rtc_unselect();
    spi_fast();
}

void rtc_read(uint8_t addr, uint8_t *dest, uint8_t len) {
    spi_slow();
    rtc_select();

    spi_transfer(MCP7951_READ);
    spi_transfer(addr);
    for (uint8_t i = 0; i < len; i++) {
        dest[i] = spi_transfer(0xff); 
    }
    
    rtc_unselect();
    spi_fast();
}

void rtc_set(datetime_t *datetime) {
    // TODO: 
    // 1. clear ST flag
    // 2. wait for OSCRUN flag to go to 0
    // 3. Load new values
    // 4. Set the ST flag again

    uint8_t rtc_buf[8];

    // Clear ST, VBAT
    rtc_buf[0] = 0;
    rtc_write(1, rtc_buf, 1);
    rtc_write(4, rtc_buf, 1);

    // Wait for oscillator to stop
    uint8_t success = 0;
    for (uint8_t i = 0; i < 255; i++) {
        rtc_read(4, &rtc_buf[0], 1);

        if ((rtc_buf[0] & _BV(5)) == 0) {
            success = 1;
            break;
        }

        _delay_ms(10);
    }

    if (!success) {
        debug("Oscillator wouldn't stop");
        return;
    }

    // Start oscillator, ST bit at address 1, bit 7
    rtc_buf[0] = 0x80;
    rtc_write(1, rtc_buf, 1);

    // Use 24-hour time, 12/24 bit at address 3, bit 6
    rtc_buf[0] = 0x40;
    rtc_write(3, rtc_buf, 1);

    // Wait for oscillator to start up
    success = 0;
    for (uint8_t i = 0; i < 255; i++) {
        rtc_read(4, &rtc_buf[0], 1);

        if (rtc_buf[0] & _BV(5)) {
            success = 1;
            break;
        }

        _delay_ms(10);
    }

    if (success) {
        // Enable battery backup, VBAT bit at address 4, bit 3
        rtc_buf[0] = 0x08;
        rtc_write(4, rtc_buf, 1);
    } else {
        debug("Oscillator never started up");
    }
}

void rtc_get(datetime_t *dest) {
    uint8_t datetime[8];

    rtc_read(0, datetime, 8);

    // Bloody BCD, why are they still using it in 2024
    uint8_t hundredths = ((datetime[0] >> 4) * 10) + (datetime[0] & 0xf);
    uint8_t started = datetime[1] >> 7;
    uint8_t seconds = (((datetime[1] >> 4) & 7) * 10) + (datetime[1] & 0xf);
    uint8_t minutes = (((datetime[2] >> 4) & 7) * 10) + (datetime[2] & 0xf);

    //uint8_t format = (datetime[3] >> 6) & 1;
    //uint8_t ampm = (datetime[3] >> 5) & 1;
    uint8_t hours = (((datetime[3] >> 4) & 1) * 10) + (datetime[3] & 0xf);

    uint8_t oscrun = (datetime[4] >> 5) & 1;
    uint8_t pwrfail = (datetime[4] >> 4) & 1;
    uint8_t vbaten = (datetime[4] >> 3) & 1;

    // Don't care about day of week

    uint8_t day = (((datetime[5] >> 4) & 3) * 10) + (datetime[5] & 0xf);
    uint8_t month = (((datetime[6] >> 4) & 1) * 10) + (datetime[6] & 0xf);
    uint8_t year = (((datetime[7] >> 4) & 0xf) * 10) + (datetime[7] & 0xf);

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
    debug_decimal(year);
    debug_nocr("-");
    
    debug_decimal(month);
    debug_nocr("-");

    debug_decimal(day);
    debug_nocr(" ");

    // Show time as HH:MM:SS.XX
    debug_decimal(hours);
    debug_nocr(":");
    
    debug_decimal(minutes);
    debug_nocr(":");

    debug_decimal(seconds);
    debug_nocr(".");

    debug_decimal(hundredths);

    debug("");
}