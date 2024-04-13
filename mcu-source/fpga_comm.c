#include <avr/io.h>
#include <util/delay_basic.h>
#include <util/delay.h>

#include "fpga_comm.h"
#include "timer0.h"
#include "debug.h"

uint8_t extra_data_byte = EXTRA_BYTE_DEFAULT;

// 0 = in
uint8_t bus_direction = 0;

// Change direction of AVR data bus
void set_data_in() {
    // Set data bus to input
    DATA_PORT.DIR = 0;

    // Set data direction pin (low = in)
    BUSDIR_PORT.OUTCLR = BUSDIR_BIT;
    bus_direction = 0;
}

// Change direction of AVR data bus
void set_data_out() {
    // Set data direction pin (high = out)
    BUSDIR_PORT.OUTSET = BUSDIR_BIT;

    // Set data bus to output
    DATA_PORT.DIR = 0xff;

    bus_direction = 1;
}

void setup() {
    // Bus direction selector (default: low which is FPGA -> arduino)
    BUSDIR_PORT.DIRSET = BUSDIR_BIT;

    set_data_in();

    // FPGA interrupt - A_INT - is an input so no config needed

    // CS (ACSI) - out
    A_CS_PORT.DIRSET = A_CS_BIT;

    // A_EXTRA, A_READY - outputs
    A_EXTRA_PORT.DIRSET = A_EXTRA_BIT;
    A_EXTRA_2_PORT.DIRSET = A_EXTRA_2_BIT;
    A_READY_PORT.DIRSET = A_READY_BIT;

    // LEDs
    GREEN_LED_PORT.DIRSET = GREEN_LED_BIT;
    RED_LED_PORT.DIRSET = RED_LED_BIT;

    // No pullups, slew rate limit, or inversions on data pins
    DATA_PORT.PINCONFIG = 0;
    DATA_PORT.PINCTRLUPD = 0xff;
}

uint8_t get_cmd() {
    return A_CMD_PORT.IN & A_CMD_BIT;
}

// TODO: 9th bit will be "first command byte" status.
uint8_t read_command_byte(uint32_t timeout) {
    set_data_in();
    set_ready();

    // Wait for A_INT
    while (get_int() == 0) {
        if (get_clock() >= timeout) {
            return 0;
        }
    }

    // Pick up byte from data bus
    uint8_t cmd_byte = read_data_port();

    clear_ready();
    strobe_cs();

    return cmd_byte;
}

// FIXME: this is actually the same as a read in command mode
uint8_t read_byte() {
    set_data_in();
    return read_byte_nochecks();
}

void set_command_mode() {
    extra_data_byte |= _BV(5);
    write_extra_byte();
    _delay_us(25);
}

void set_data_mode() {
    extra_data_byte &= ~_BV(5);
    write_extra_byte();
    _delay_us(25);
}

void set_extra_data() {
    A_EXTRA_PORT.OUTSET = A_EXTRA_BIT;
}

void clear_extra_data() {
    A_EXTRA_PORT.OUTCLR = A_EXTRA_BIT;
}

void set_extra2_data() {
    A_EXTRA_2_PORT.OUTSET = A_EXTRA_2_BIT;
}

void clear_extra2_data() {
    A_EXTRA_2_PORT.OUTCLR = A_EXTRA_2_BIT;
}

// Flush the extra data byte to the FPGA to update its config.
// I am using A_X HIGH to represent a write to extra_data_byte
void write_extra_byte() {

    // Data bus direction: AVR to FPGA
    uint8_t saved_bus_direction = bus_direction;

    set_extra_data();
    set_data_out();

    // drive the bus
    write_data_port(extra_data_byte);

    // Strobe data
    strobe_cs();

    clear_extra_data();

    // Restore bus direction, if needed
    if (saved_bus_direction == 0) {
        set_data_in();
    }
}

// Update the ACSI IDs
void set_acsi_ids(uint8_t ids) {

    // Data bus direction: AVR to FPGA
    uint8_t saved_bus_direction = bus_direction;

    set_extra2_data();
    set_data_out();

    // drive the bus
    write_data_port(ids);

    // Strobe data
    strobe_cs();

    clear_extra2_data();

    // Restore bus direction, if needed
    if (saved_bus_direction == 0) {
        set_data_in();
    }
}

void green_led_on() {
    GREEN_LED_PORT.OUTSET = GREEN_LED_BIT;
}

void red_led_on() {
    RED_LED_PORT.OUTSET = RED_LED_BIT;
}

void green_led_off() {
    GREEN_LED_PORT.OUTCLR = GREEN_LED_BIT;
}

void red_led_off() {
    RED_LED_PORT.OUTCLR = RED_LED_BIT;
}

// The last operation of any command response
void send_status(uint8_t status) {
    debug_nocr("STATUS: ");
    debug_decimal(status);
    debug("");
    
    set_data_out();
    set_command_mode();
    write_byte(status);
    // Wait for A_INT high so we know the byte was read
    _delay_us(5); // Maybe not needed
    while (get_int() == 0)
        ;
    // Go back to command/read
    set_data_in();
}
