#include "fpga_comm.h"
#include "timer0.h"
#include "debug.h"

#include "stdutil.h"

uint8_t extra_data_byte = EXTRA_BYTE_DEFAULT;

// 0 = in
uint8_t bus_direction = 0;

// Change direction of AVR data bus
void set_data_in() {
    // Set data bus to input
    DATA_PORT->MODER &= DATA_PORT_MASK;

    // Set data direction pin (low = in)
    BUSDIR_PORT->BSRR = BSR_LOW(BUSDIR_BIT);
    bus_direction = 0;
}

// Change direction of AVR data bus
void set_data_out() {
    // Set data direction pin (high = out)
    BUSDIR_PORT->BSRR = BSR_HIGH(BUSDIR_BIT);

    // Set data bus to output
    DATA_PORT->MODER &= DATA_PORT_MASK;
    //DATA_PORT->MODER |= MODE_OUTPUT(0) | MODE_OUTPUT(1) | MODE_OUTPUT(2) | MODE_OUTPUT(3) 
    //    | MODE_OUTPUT(4) | MODE_OUTPUT(5) | MODE_OUTPUT(6) | MODE_OUTPUT(7);
    DATA_PORT->MODER |= 0x00005555;

    bus_direction = 1;
}

void setup() {
    // Bus direction selector (default: low which is FPGA -> arduino)
    BUSDIR_PORT->MODER &= MODE_MASK(BUSDIR_BIT);
    BUSDIR_PORT->MODER |= MODE_OUTPUT(BUSDIR_BIT);
    BUSDIR_PORT->OTYPER &= OTYPE_MASK(BUSDIR_BIT);
    BUSDIR_PORT->OSPEEDR &= OSPEED_MASK(BUSDIR_BIT);
    BUSDIR_PORT->OSPEEDR |= OSPEED_VFAST(BUSDIR_BIT);    
    set_data_in();

    // FPGA interrupt - A_INT (input)
    A_INT_PORT->MODER &= MODE_MASK(A_INT_BIT);

    // A_CMD
    A_CMD_PORT->MODER &= MODE_MASK(A_CMD_BIT);

    // CS (ACSI) - out
    A_CS_PORT->MODER &= MODE_MASK(A_CS_BIT);
    A_CS_PORT->MODER |= MODE_OUTPUT(A_CS_BIT);
    A_CS_PORT->OTYPER &= OTYPE_MASK(A_CS_BIT);
    A_CS_PORT->OSPEEDR &= OSPEED_MASK(A_CS_BIT);
    A_CS_PORT->OSPEEDR |= OSPEED_VFAST(A_CS_BIT);       

    // A_EXTRA, A_EXTRA2, A_READY - outputs
    // A_EXTRA means a write to the extra data byte
    A_EXTRA_PORT->MODER &= MODE_MASK(A_EXTRA_BIT);
    A_EXTRA_PORT->MODER |= MODE_OUTPUT(A_EXTRA_BIT);
    A_EXTRA_PORT->OTYPER &= OTYPE_MASK(A_EXTRA_BIT);
    A_EXTRA_PORT->OSPEEDR &= OSPEED_MASK(A_EXTRA_BIT);
    A_EXTRA_PORT->OSPEEDR |= OSPEED_VFAST(A_EXTRA_BIT); 

    // A_EXTRA_2  means a write to the ACSI IDs byte
    A_EXTRA_2_PORT->MODER &= MODE_MASK(A_EXTRA_2_BIT);
    A_EXTRA_2_PORT->MODER |= MODE_OUTPUT(A_EXTRA_2_BIT);
    A_EXTRA_2_PORT->OTYPER &= OTYPE_MASK(A_EXTRA_2_BIT);
    A_EXTRA_2_PORT->OSPEEDR &= OSPEED_MASK(A_EXTRA_2_BIT);
    A_EXTRA_2_PORT->OSPEEDR |= OSPEED_VFAST(A_EXTRA_2_BIT); 

    A_READY_PORT->MODER &= MODE_MASK(A_READY_BIT);
    A_READY_PORT->MODER |= MODE_OUTPUT(A_READY_BIT);
    A_READY_PORT->OTYPER &= OTYPE_MASK(A_READY_BIT);
    A_READY_PORT->OSPEEDR &= OSPEED_MASK(A_READY_BIT);
    A_READY_PORT->OSPEEDR |= OSPEED_VFAST(A_READY_BIT);
    clear_extra_data();
    clear_extra2_data();
    clear_ready();

    // LEDs
    // Put pin 13 (red LED) in general purpose push-pull mode
    RED_LED_PORT->MODER &= MODE_MASK(RED_LED_BIT);
    RED_LED_PORT->MODER |= MODE_OUTPUT(RED_LED_BIT);
    RED_LED_PORT->OTYPER &= OTYPE_MASK(RED_LED_BIT);

    // Same with pin 14 (green LED)
    GREEN_LED_PORT->MODER &= MODE_MASK(GREEN_LED_BIT);
    GREEN_LED_PORT->MODER |= MODE_OUTPUT(GREEN_LED_BIT);
    GREEN_LED_PORT->OTYPER &= OTYPE_MASK(GREEN_LED_BIT);

    // Slow speed is fine
    RED_LED_PORT->OSPEEDR &= OSPEED_MASK(RED_LED_BIT);
    RED_LED_PORT->OSPEEDR |= OSPEED_SLOW(RED_LED_BIT);
    GREEN_LED_PORT->OSPEEDR &= OSPEED_MASK(GREEN_LED_BIT);
    GREEN_LED_PORT->OSPEEDR |= OSPEED_SLOW(GREEN_LED_BIT);

    // Switch off both LED pins
    red_led_off();
    green_led_off();

    // Data pins PORTA0-PORTA7
    // Speed: very fast
    DATA_PORT->OSPEEDR |= OSPEED_VFAST(0) | OSPEED_VFAST(1) | OSPEED_VFAST(2) | OSPEED_VFAST(3) 
        | OSPEED_VFAST(4) | OSPEED_VFAST(5) | OSPEED_VFAST(6) | OSPEED_VFAST(7);

    // Enable input mode, pull-ups on SD card detect lines
    SDCARD0_DETECT_PORT->MODER &= MODE_MASK(SDCARD0_DETECT_BIT);
    SDCARD0_DETECT_PORT->PUPDR &= PUPD_MASK(SDCARD0_DETECT_BIT);
    SDCARD0_DETECT_PORT->PUPDR |= PUPD_PULLUP(SDCARD0_DETECT_BIT);

    SDCARD1_DETECT_PORT->MODER &= MODE_MASK(SDCARD1_DETECT_BIT);
    SDCARD1_DETECT_PORT->PUPDR &= PUPD_MASK(SDCARD1_DETECT_BIT);
    SDCARD1_DETECT_PORT->PUPDR |= PUPD_PULLUP(SDCARD1_DETECT_BIT);

    SDCARD1_WP_PORT->MODER &= MODE_MASK(SDCARD1_WP_BIT);
    SDCARD1_WP_PORT->PUPDR &= PUPD_MASK(SDCARD1_WP_BIT);
    SDCARD1_WP_PORT->PUPDR |= PUPD_PULLUP(SDCARD1_WP_BIT);
}

uint8_t get_cmd() {
    //return A_CMD_PORT.IN & A_CMD_BIT;
    return 0;
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
    A_EXTRA_PORT->BSRR = BSR_HIGH(A_EXTRA_BIT);
}

void clear_extra_data() {
    A_EXTRA_PORT->BSRR = BSR_LOW(A_EXTRA_BIT);
}

void set_extra2_data() {
    A_EXTRA_2_PORT->BSRR = BSR_HIGH(A_EXTRA_2_BIT);
}

void clear_extra2_data() {
    A_EXTRA_2_PORT->BSRR = BSR_LOW(A_EXTRA_2_BIT);
}

// Flush the extra data byte to the FPGA to update its config.
// I am using A_X HIGH to represent a write to extra_data_byte
void write_extra_byte() {

    // Data bus direction: AVR to FPGA
    uint8_t saved_bus_direction = bus_direction;

    clear_extra2_data();
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

    clear_extra_data();
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
    GREEN_LED_PORT->BSRR = BSR_HIGH(GREEN_LED_BIT);
}

void red_led_on() {
    RED_LED_PORT->BSRR = BSR_HIGH(RED_LED_BIT);
}

void green_led_off() {
    GREEN_LED_PORT->BSRR = BSR_LOW(GREEN_LED_BIT);
}

void red_led_off() {
    RED_LED_PORT->BSRR = BSR_LOW(RED_LED_BIT);
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

uint_fast8_t sdcard0_present() {
    return SDCARD0_DETECT_PORT->IDR & _BV(SDCARD0_DETECT_BIT) ? 0 : 1;
}

uint_fast8_t sdcard1_present() {
    return SDCARD1_DETECT_PORT->IDR & _BV(SDCARD1_DETECT_BIT) ? 0 : 1;
}

uint_fast8_t sdcard1_write_protected() {
    return SDCARD1_WP_PORT->IDR & _BV(SDCARD1_WP_BIT) ? 0 : 1;
}