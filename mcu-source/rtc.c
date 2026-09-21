#include <string.h>

#include "stm32c0xx.h"
#include "spi.h"
#include "rtc.h"
#include "debug.h"
#include "timer0.h"

#include "stdutil.h"

void rtc_select() {
    RTC_SS_PORT->BSRR = BSR_LOW(RTC_SS_PIN);
    //_delay_ms(1);
}

void rtc_unselect() {
    RTC_SS_PORT->BSRR = BSR_HIGH(RTC_SS_PIN);
    //_delay_ms(1);
}

void rtc_setup() {
    // AI version
    // ==========================================
    // 1. ENABLE CLOCKS FIRST (CRITICAL)
    // ==========================================
    RCC->IOPENR  |= (RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN);
    RCC->APBENR1 |= RCC_APBENR1_USART2EN;

    // Unselect the RTC line early so it does not float during boot
    rtc_unselect();
    _delay_ms(1);
    rtc_select();
    _delay_ms(1);
    rtc_unselect();
    _delay_ms(1);

    // Disable interrupts on USART2 line during routing
    NVIC_DisableIRQ(USART2_IRQn);

    // ==========================================
    // 2. CONFIGURE GPIO & ALTERNATE FUNCTIONS
    // ==========================================
    // Setup SS pin as output
    RTC_SS_PORT->MODER &= MODE_MASK(RTC_SS_PIN);
    RTC_SS_PORT->MODER |= MODE_OUTPUT(RTC_SS_PIN);
    RTC_SS_PORT->OSPEEDR &= OSPEED_MASK(RTC_SS_PIN);
    RTC_SS_PORT->OSPEEDR |= OSPEED_VFAST(RTC_SS_PIN);

    // // Clear and set Alternate Function mappings (Pins 8, 15 use AFR[1])
    // RTC_MOSI_PORT->AFR[1] &= ~(0b1111 << (4 * (RTC_MOSI_BIT - 8)));
    // RTC_MOSI_PORT->AFR[1] |= (14 << (4 * (RTC_MOSI_BIT - 8))); // PA8 -> AF14 (USART2_TX)

    // RTC_MISO_PORT->AFR[1] &= ~(0b1111 << (4 * (RTC_MISO_BIT - 8)));
    // RTC_MISO_PORT->AFR[1] |= (1 << (4 * (RTC_MISO_BIT - 8)));   // PA15 -> AF1 (USART2_RX)

    // RTC_CLK_PORT->AFR[1]  &= ~(0b1111 << (4 * (RTC_CLK_BIT - 8)));
    // RTC_CLK_PORT->AFR[1]  |= (1 << (4 * (RTC_CLK_BIT - 8)));    // PB9 -> AF1 (USART2_CK)

    // MOSI is on PA8 -> Mapped to AF1 for USART2_TX (NOT AF14)
    RTC_MOSI_PORT->AFR[1] &= ~(0b1111 << (4 * (RTC_MOSI_BIT - 8)));
    RTC_MOSI_PORT->AFR[1] |= (1 << (4 * (RTC_MOSI_BIT - 8)));    // <-- Change 14 to 1

    // MISO is on PA15 -> Mapped to AF4 or AF9 for USART2_RX (NOT AF1)
    // Check your package, but AF4 / AF9 connects the hardware routing pipeline cleanly.
    RTC_MISO_PORT->AFR[1] &= ~(0b1111 << (4 * (RTC_MISO_BIT - 8)));
    RTC_MISO_PORT->AFR[1] |= (1 << (4 * (RTC_MISO_BIT - 8)));    // <-- Change 1 to 4 (or 9)

    // CLK is on PB9 -> Mapped to AF1 for USART2_CK (This line was already correct!)
    RTC_CLK_PORT->AFR[1]  &= ~(0b1111 << (4 * (RTC_CLK_BIT - 8)));
    RTC_CLK_PORT->AFR[1]  |= (1 << (4 * (RTC_CLK_BIT - 8)));     // PB9 -> AF1

    // Assign Pins to Alternate Function Mode using your fixed macros
    RTC_MOSI_PORT->MODER &= MODE_MASK(RTC_MOSI_BIT);
    RTC_MOSI_PORT->MODER |= MODE_ALT_FUNC(RTC_MOSI_BIT);
    
    RTC_MISO_PORT->MODER &= MODE_MASK(RTC_MISO_BIT);
    RTC_MISO_PORT->MODER |= MODE_ALT_FUNC(RTC_MISO_BIT);
    
    RTC_CLK_PORT->MODER  &= MODE_MASK(RTC_CLK_BIT);
    RTC_CLK_PORT->MODER  |= MODE_ALT_FUNC(RTC_CLK_BIT);

    // Push speeds to Very Fast for high edge integrity
    // RTC_MOSI_PORT->OSPEEDR &= OSPEED_MASK(RTC_MOSI_BIT);
    // RTC_MOSI_PORT->OSPEEDR |= OSPEED_VFAST(RTC_MOSI_BIT);
    // RTC_MISO_PORT->OSPEEDR &= OSPEED_MASK(RTC_MISO_BIT);
    // RTC_MISO_PORT->OSPEEDR |= OSPEED_VFAST(RTC_MISO_BIT);
    // RTC_CLK_PORT->OSPEEDR  &= OSPEED_MASK(RTC_CLK_BIT);
    // RTC_CLK_PORT->OSPEEDR  |= OSPEED_VFAST(RTC_CLK_BIT);

    // ==========================================
    // 3. CONFIGURE USART2 AS SPI MASTER
    // ==========================================
    // Ensure USART2 is entirely offline to allow structural changes
    USART2->CR1 = 0; 
    USART2->CR2 = 0;

    // Configure CR2: Synchronous Enable, Mode 0, MSB First, AND Last Bit Clock Pulse (LBCL)
    USART2->CR2 |= (USART_CR2_CLKEN | USART_CR2_MSBFIRST | USART_CR2_LBCL);
    USART2->CR2 &= ~(USART_CR2_CPOL | USART_CR2_CPHA); // Explicit Mode 0

    // Setup division baud scaling (e.g., 16,000,000 / 100,000 = 160)
    USART2->BRR = F_PCLK / 100000L;

    // Turn ON the system core along with receiver and transmitter engines
    USART2->CR1 |= (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE);     

    // Enable vector interrupt hooks if desired
    NVIC_EnableIRQ(USART2_IRQn);

    return;
    // // Use Alternate Function on MOSI/MISO/CLK
    // RTC_MOSI_PORT->MODER &= ~(0b11 << (RTC_MOSI_BIT * 2));
    // RTC_MOSI_PORT->MODER |= (2 << (RTC_MOSI_BIT * 2));

    // RTC_MISO_PORT->MODER &= ~(0b11 << (RTC_MISO_BIT * 2));
    // RTC_MISO_PORT->MODER |= (2 << (RTC_MISO_BIT * 2));

    // RTC_CLK_PORT->MODER &= ~(0b11 << (RTC_CLK_BIT * 2));
    // RTC_CLK_PORT->MODER |= (2 << (RTC_CLK_BIT * 2));


    // // Setup SS pin as output
    // RTC_SS_PORT->MODER &= MODE_MASK(RTC_SS_PIN);
    // RTC_SS_PORT->MODER |= MODE_OUTPUT(RTC_SS_PIN);
    // //RTC_SS_PORT->OTYPER &= OTYPE_MASK(RTC_SS_PIN);
    // //RTC_SS_PORT->OTYPER |= OTYPE_OPEN_DRAIN(RTC_SS_PIN);

    // RTC_SS_PORT->OSPEEDR &= OSPEED_MASK(RTC_SS_PIN);
    // RTC_SS_PORT->OSPEEDR |= OSPEED_VFAST(RTC_SS_PIN);

    // rtc_unselect();

    // _delay_ms(1);

    // // We're not going to use interrupts, but disable them during setup nonetheless
    // NVIC_DisableIRQ(USART2_IRQn);

    // // ---- Setup USART2 as SPI controller

    // // // Disable UART2 peripheral
    // // USART2->CR1 &= ~USART_CR1_UE;

    // // // Enable clock in synchronous mode, MSB first
    // // USART2->CR2 = USART_CR2_CLKEN | USART_CR2_MSBFIRST;


    // // 1. Disable USART2 while configuring
    // USART2->CR1 &= ~USART_CR1_UE;

    // // 2. Set Word length to 8-bit (M1=0, M0=0), Disable Parity Control
    // USART2->CR1 &= ~(USART_CR1_M1 | USART_CR1_M0 | USART_CR1_PCE);

    // // 3. Configure CR2 for Synchronous SPI Master Mode 0 & LSB First
    // USART2->CR2 &= ~(USART_CR2_CPOL | USART_CR2_CPHA); // Mode 0
    // USART2->CR2 |= USART_CR2_CLKEN;                   // Enable SCLK on PB9
    // USART2->CR2 |= USART_CR2_LBCL;                    // Emit one extra clock pulse at the end of the transfer
    // USART2->CR2 |= USART_CR2_MSBFIRST;                // MCP7951x relies on LSB First

    // // 4. Configure Baud Rate (Assuming PCLK = 16 MHz HSI)
    // // To get a safe 1 MHz SPI Clock: BRR = 16,000,000 / 1,000,000 = 16
   
    // // Setup baud rate
    // USART2->BRR = F_PCLK / 100000L;

    // // 3. Configure USART_CR1: 8 data bits, no parity, enable TX and RX
    // USART2->CR1 = USART_CR1_TE | USART_CR1_RE;     

    // // Word length is 8 bits by default

    // // Parity is off by default

    // // 1 stop bits by default - but this only adds a delay and not a clock pulse

    // // RTS/CTS disabled by default

    // // Enable the transmitter and receiver
    // // USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;  

    // // Note: hard-coded register number due to there being 8 per register
    // // Clear these first, then set correct Alternate Function
    // // ...MOSI is on PA8, alternate function 14
    // RTC_MOSI_PORT->AFR[1] &= ~(0b1111 << (4 * (RTC_MOSI_BIT - 8)));
    // RTC_MOSI_PORT->AFR[1] |= (14 << (4 * (RTC_MOSI_BIT - 8)));

    // // MISO is on PA15, alternate function 1
    // RTC_MISO_PORT->AFR[1] &= ~(0b1111 << (4 * (RTC_MISO_BIT - 8)));
    // RTC_MISO_PORT->AFR[1] |= (1 << (4 * (RTC_MISO_BIT - 8)));

    // // CLK is on PB9, Alternate Function 1
    // RTC_CLK_PORT->AFR[1] &= ~(0b1111 << (4 * (RTC_CLK_BIT - 8)));
    // RTC_CLK_PORT->AFR[1] |= (1 << (4 * (RTC_CLK_BIT - 8)));

    // // Select Alternate Function for the GPIO pin modes
    // // RTC_MOSI_PORT->MODER &= MODE_MASK(RTC_MOSI_BIT);
    // // RTC_MOSI_PORT->MODER |= MODE_ALT_FUNC(RTC_MOSI_BIT);
    // // RTC_MISO_PORT->MODER &= MODE_MASK(RTC_MISO_BIT);
    // // RTC_MISO_PORT->MODER |= MODE_ALT_FUNC(RTC_MISO_BIT);
    // // RTC_CLK_PORT->MODER &= MODE_MASK(RTC_CLK_BIT);
    // // RTC_CLK_PORT->MODER |= MODE_ALT_FUNC(RTC_CLK_BIT);

    // // Speed them up
    // RTC_MOSI_PORT->OSPEEDR &= OSPEED_MASK(RTC_MOSI_BIT);
    // RTC_MOSI_PORT->OSPEEDR |= OSPEED_VFAST(RTC_MOSI_BIT);
    // RTC_MISO_PORT->OSPEEDR &= OSPEED_MASK(RTC_MISO_BIT);
    // RTC_MISO_PORT->OSPEEDR |= OSPEED_VFAST(RTC_MISO_BIT);
    // RTC_CLK_PORT->OSPEEDR &= OSPEED_MASK(RTC_CLK_BIT);
    // RTC_CLK_PORT->OSPEEDR |= OSPEED_VFAST(RTC_CLK_BIT);

    // // Enable USART2 peripheral
    // //USART2->CR1 |= USART_CR1_UE;

    // // 5. Enable USART2, Transmitter, and Receiver
    // USART2->CR1 |= (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE);    

    // NVIC_EnableIRQ(USART2_IRQn);
}

uint8_t rtc_txrx(uint8_t data) {
    // // Wait for idle
    // while ((USART2->ISR & USART_ISR_TXE_TXFNF) == 0)
    //     ;
        
    // // Put data into buffer, sends the data
    // USART2->TDR = data;

    // // Wait for received data register to become full
    // while ((USART2->ISR & USART_ISR_RXNE_RXFNE) == 0)
    //     ;
    // // Somehow we always read 0x7f here
    // return USART2->RDR;

    // 1. Ensure the transmit register/FIFO is ready for a new byte
    while (!(USART2->ISR & USART_ISR_TXE_TXFNF));
    
    // 2. Perform an 8-bit write to TDR (forces the USART to send exactly 1 frame/8 clock cycles)
    *((__IO uint8_t *)&USART2->TDR) = data;
    
    // 3. Wait until the hardware pulls down the bits into the Receive FIFO
    while (!(USART2->ISR & USART_ISR_RXNE_RXFNE));
    
    // 4. Perform an 8-bit read from RDR
    return *((__IO uint8_t *)&USART2->RDR);    
}

// Write some bytes into the clock memory
void rtc_write(uint8_t addr, uint8_t *data, uint8_t len) {

    rtc_select();

    // debug("RTC write command");
    rtc_txrx(MCP7951_WRITE);
    // debug("RTC write address");
    rtc_txrx(addr); // address

    // debug("RTC write data");
    for (uint8_t i = 0; i < len; i++) {
      rtc_txrx(data[i]);
    }

    // debug("RTC write command completed");

    rtc_unselect();
}

void rtc_read(uint8_t addr, uint8_t *dest, uint8_t len) {
    rtc_unselect();
    rtc_select();

    // debug_nocr("RTC read command (");
    // debug_decimal(len);
    // debug(" bytes)");
    rtc_txrx(MCP7951_READ);
    // debug("RTC read address");
    rtc_txrx(addr);
    // debug("RTC read data");
    for (uint8_t i = 0; i < len; i++) {
        uint8_t temp = rtc_txrx(0xff);
        dest[i] = temp;

        // debug_nocr("RTC read - byte");
        // debug_decimal(i);
        // debug_nocr(" [");
        // debug_decimal(temp);
        // debug("]\n");
    }
    // debug("RTC read command completed");
    
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

    uint8_t rtc_buf[8];

    // Clear ST and VBAT flags
    // GCC thinks this leaves the memory uninitialised:
    memset(rtc_buf, 0, 8);
    //spi_slow();

    rtc_write(1, rtc_buf, 4); // Clears ST bit in addr 1 and VBATEN bit in addr 4
    debug("Wrote stop-osc command");

    // Wait for oscillator to stop running
    uint8_t success = 0;
    for (uint8_t i = 0; i < 25; i++) {
        rtc_read(4, &rtc_buf[0], 1);

        debug_nocr("Read ");
        debug_hex(rtc_buf[0], 2);
        debug(" from addr 4");

        if ((rtc_buf[0] & _BV(5)) == 0) {
            // Oscillator is not running.
            success = 1;
            break;
        }

        _delay_ms(100);
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

        // debug_nocr("Read ");
        // debug_hex(rtc_buf[0], 2);
        // debug(" from addr 4");        

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
    uint8_t rtc_buf[8]; // = {0, 0, 0, 0, 0, 0, 0, 0};
    memset(rtc_buf, 0, 8);
    //spi_slow();

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