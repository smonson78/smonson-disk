#ifndef __FPGA_COMM_H
#define __FPGA_COMM_H

#include "stm32c0xx.h"

#include <stdint.h>
#include "stdutil.h"

#define DEBUG

// For the AVR <==> CPLD bus
#define BUSDIR_PORT GPIOB
#define BUSDIR_BIT 1

#define DATA_PORT GPIOA
//#define DATA_PORT_MASK (MODE_MASK(0) & MODE_MASK(1) & MODE_MASK(2) & MODE_MASK(3) & MODE_MASK(4) & MODE_MASK(5) & MODE_MASK(6) & MODE_MASK(7))
#define DATA_PORT_MASK 0xffff0000

#define A_INT_PORT GPIOF
#define A_INT_BIT 1

#define A_CMD_PORT GPIOB
#define A_CMD_BIT 2

#define A_CS_PORT GPIOB
#define A_CS_BIT 0

#define A_EXTRA_PORT GPIOB
#define A_EXTRA_BIT 11

#define A_EXTRA_2_PORT GPIOB
#define A_EXTRA_2_BIT 10

#define A_READY_PORT GPIOF
#define A_READY_BIT 0

// Front panel LEDs
#define RED_LED_PORT GPIOC
#define RED_LED_BIT 14

#define GREEN_LED_PORT GPIOC
#define GREEN_LED_BIT 13

// SD card-detect
#define SDCARD0_DETECT_PORT GPIOC
#define SDCARD0_DETECT_BIT 6

#define SDCARD1_DETECT_PORT GPIOA
#define SDCARD1_DETECT_BIT 11

#define SDCARD1_WP_PORT GPIOA
#define SDCARD1_WP_BIT 12

// Data bus direction:
// HIGH: A (Arduino) --> B (Atari)
// LOW: B (Atari) --> A (Arduino)

#define DEFAULT_DEVICE_NUM 0

// Extra data byte:
// bit 7     - Not used
// bit 6     - Manually deselect device from bus if bit set
// bit 5     - HIGH=in command mode LOW=in data mode
// bit 4-0   - Not used
#define EXTRA_BYTE_DEFAULT 0b00100000
extern uint8_t extra_data_byte;

// 0 = in
extern uint8_t bus_direction;

// Setup pin directions and initial condition
void setup();

// Set AVR bus direction:
//   IN  = FPGA --> AVR
//   OUT = FPGA <-- AVR
void set_data_in();
void set_data_out();

// Indicate AVR readiness:
// CLEAR - AVR is not ready
// SET   - AVR is ready
static inline void clear_ready() {
    A_READY_PORT->BSRR = BSR_LOW(A_READY_BIT);
}

static inline void set_ready() {
    A_READY_PORT->BSRR = BSR_HIGH(A_READY_BIT);
}

// Get the value on the data bus
static inline uint8_t read_data_port() {
    return DATA_PORT->IDR;
}

// Get the value of the A_CMD pin which is asserted by the FPGA when presenting the first byte of a new command
// This means the A1 pin of the ACSI bus was low at the time the byte was read.
uint8_t get_cmd();

// Get the status of the A_INT pin which is asserted by the FPGA to indicate readiness
static inline uint8_t get_int() {
    return A_INT_PORT->IDR & _BV(A_INT_BIT);
}

// Raise, then lower, the A_CS signal
//   - (when writing) - to cause the FPGA to latch the contents of the data bus
//   - (when reading) - to cause the FPGA to see that the value on the bus has been read by the AVR
static inline void strobe_cs() {
    A_CS_PORT->BSRR = BSR_HIGH(A_CS_BIT);
    A_CS_PORT->BSRR = BSR_HIGH(A_CS_BIT);
    // This can't be too fast or it'll be missed by the FPGA
    // (assuming the FPGA is synchronised by a clock)
    A_CS_PORT->BSRR = BSR_LOW(A_CS_BIT);
    A_CS_PORT->BSRR = BSR_LOW(A_CS_BIT);
}

// Read a byte from the FPGA
uint8_t read_byte();

static inline uint8_t read_byte_nochecks() {
    set_ready();

    // wait for INT high
    while (get_int() == 0)
        ;

    uint8_t value = read_data_port();

    clear_ready();
    strobe_cs();

    return value;
}

static inline void write_data_port(uint8_t value) {
    // Write only low 8 bits
    BYTE_ACCESS(DATA_PORT->ODR) = value;
}

// Write a byte without requesting the next
static inline void write_byte_nochecks(uint8_t value) {
    write_data_port(value);
    // wait for INT high
    while (get_int() == 0)
        ;
    strobe_cs();
}

// Write a byte to the FPGA, requesting the next
static inline void write_byte(uint8_t value) {
    set_data_out();
    write_byte_nochecks(value);
}

// Request, then wait for a command byte. The request signals the Atari to send a byte via the ACSI pin IRQ
uint8_t read_command_byte(uint32_t timeout);

// Write a status byte to the Atari
void send_status(uint8_t status);

// Set the ACSI bus protocol mode to command or data. This affects the signalling mechanism between the FPGA and Atari
void set_command_mode();
void set_data_mode();

// Set or unset the Extra Data pin which controls reads and writes from extra data registers
void set_extra_data();
void clear_extra_data();
void set_extra2_data();
void clear_extra2_data();

// Write "extra byte" data to the FPGA - this contains device config registers
// Note the extra byte is a global variable so isn't passed into the function.
void write_extra_byte();

// Set ACSI IDs that this device will respond to (bit mask)
void set_acsi_ids(uint8_t ids);

// Turn the LEDs on or off
void green_led_on();
void green_led_off();
void red_led_on();
void red_led_off();

// Enable/disable SPI pass-through on the FPGA so that the AVR can communicate directly with it
void enable_spi();
void disable_spi();

uint_fast8_t sdcard0_present();
uint_fast8_t sdcard1_present();
uint_fast8_t sdcard1_write_protected();

#endif