#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay_basic.h>
#include <util/delay.h>
#include <stdint.h>

#include "sdcard.h"
#include "commands.h"
#include "acsi.h"
#include "fpga_comm.h"
#include "serial.h"
#include "timer0.h"
#include "debug.h"
#include "spi.h"
#include "rtc.h"

/*
 * "Smonson Disk" - Atari ST ACSI hard drive interface (to two SD cards and a real-time clock)
 * Simon Krix © 2024
 * 
 * License: GPL 3 or later https://www.gnu.org/licenses/gpl-3.0.en.html unless otherwise noted.
 */

// Switch internal clock to 20MHz mode
void clk_20MHz() {
    // Magic number to allow clock changes
    //CCP = CCP_IOREG_gc;

    // Switch to internal high-frequency oscillator
    //CLKCTRL.MCLKCTRLA = 0b00000000; // It's already the default

    // Set internal oscillator to run at 20MHz
    CCP = CCP_IOREG_gc;
    CLKCTRL.OSCHFCTRLA = 0b00100000;

    // Magic number to allow clock changes
    CCP = CCP_IOREG_gc;

    // Prescaler disabled
    CLKCTRL.MCLKCTRLB = 0b00000000;    
}

// Switch internal clock to 20MHz mode
void clk_24MHz() {
    // Magic number to allow clock changes
    //CCP = CCP_IOREG_gc;

    // Switch to internal high-frequency oscillator
    //CLKCTRL.MCLKCTRLA = 0b00000000; // It's already the default

    // Set internal oscillator to run at 24MHz
    CCP = CCP_IOREG_gc;
    CLKCTRL.OSCHFCTRLA = 0b00100100;

    // Magic number to allow clock changes
    CCP = CCP_IOREG_gc;

    // Prescaler disabled
    CLKCTRL.MCLKCTRLB = 0b00000000;    
}

void set_acsi_id_mask() {
    uint8_t acsi_id_mask = 0;
    if (logical_drive[0].sdcard->usable) {
        acsi_id_mask |= (1 << logical_drive[0].acsi_id);
    }
    if (logical_drive[1].sdcard->usable) {
        acsi_id_mask |= (1 << logical_drive[1].acsi_id);
    }
    debug_nocr("Setting ACSI mask to ");
    debug_hex(acsi_id_mask, 2);
    debug("");
    set_acsi_ids(acsi_id_mask);
}

int main() {
    // Setup device pins
    setup();

    // Speed up the clock to 20MHz
    clk_20MHz();

    serial_init();

    // Debug during startup
    debug_level = 5;

    init_clock();
        
    debug("\n\r\n\r\n\r-- Startup");

    // Setup SPI pins
    spi_setup();
    sdcard_setup();
    rtc_setup();

    // Settle down for a bit
    _delay_ms(10);

    datetime_t datetime;
    rtc_get(&datetime);

    // TODO: Print datetime for console here instead of in rtc_read

    // Flash both lights to indicate startup
    for (uint8_t i = 0; i < 2; i++) {
        red_led_on();
        green_led_on();
        _delay_ms(250);
        red_led_off();
        green_led_off();
        _delay_ms(250);
    }

    // Reset FPGA registers to default
    // This sets command mode, and ACSI bus direction IN
    extra_data_byte = EXTRA_BYTE_DEFAULT;
    write_extra_byte();

    // AVR bus pins to default
    clear_extra_data();
    clear_extra2_data();
    clear_ready();
    set_data_in();
    strobe_cs(); // Ensures CS is deasserted (afterwards)

    // Set defaults for SD cards
    sdcard_defaults(&sdcards[0], 0);
    sdcard_defaults(&sdcards[1], 1);
    
    // Default logical device info
    logical_drive[0].acsi_id = 0;
    logical_drive[0].sdcard = &sdcards[0];
    logical_drive[0].sense_key = 0;

    logical_drive[1].acsi_id = 1;
    logical_drive[1].sdcard = &sdcards[1];
    logical_drive[1].sense_key = 0;

    set_acsi_id_mask();

    // Now turn debug down until requested
    debug_level = 5;
    
    // Wait to read data
    set_data_in();

    while (1) {

        uint8_t is_cmd;
        uint8_t cmd_byte;
        set_data_in();
        do {
            // Wait for A_INT
            while (get_int() == 0) {
                // Check if debug level was changed by typing 0-9 on the serial port
                int16_t serial_in = serial_receive_nowait();
                if (serial_in >= 0) {
                    if (serial_in >= '0' && serial_in <= '9') {
                        debug_level = serial_in - '0';
                        serial_send_progmem(PSTR("Debug level set to: "));
                        serial_sendchar('0' + debug_level);
                        serial_send_progmem(PSTR("\r\n"));
                    }
                }

                // Check if SD cards were removed
                if (sdcards[0].detected && (SDCARD0_DETECT_PORT.IN & SDCARD0_DETECT_BIT)) {
                    debug("SD card 0 removed");
                    sdcard_defaults(&sdcards[0], 0);
                    logical_drive[0].media_changed = 1;
                    set_acsi_id_mask();
                }
                if (sdcards[1].detected && (SDCARD1_DETECT_PORT.IN & SDCARD1_DETECT_BIT)) {
                    debug("SD card 1 removed");
                    sdcard_defaults(&sdcards[1], 1);
                    logical_drive[1].media_changed = 1;
                    set_acsi_id_mask();
                }

                // Check if SD cards were inserted
                if (sdcards[0].detected == 0 && (SDCARD0_DETECT_PORT.IN & SDCARD0_DETECT_BIT) == 0) {
                    debug("SD card 0 inserted");
                    sdcards[0].detected = 1;
                    sdcard_init(&sdcards[0]);
                    logical_drive[0].media_changed = 1;
                    set_acsi_id_mask();
                }
                if (sdcards[1].detected == 0 && (SDCARD1_DETECT_PORT.IN & SDCARD1_DETECT_BIT) == 0) {
                    debug("SD card 1 inserted");
                    sdcards[1].detected = 1;
                    sdcard_init(&sdcards[1]);
                    logical_drive[1].media_changed = 1;
                    set_acsi_id_mask();
                }
            }

            // Pick up byte from data bus and A_CMD pin
            is_cmd = A_CMD_PORT.IN & A_CMD_BIT;
            cmd_byte = read_data_port();

            strobe_cs();
        } while (!is_cmd);

        // Wait for the first byte of a command to arrive
        uint8_t acsi_device = cmd_byte >> 5;
        acsi_opcode_t opcode = cmd_byte & 0x1f;

        // Clock rate is 100Hz. Find command phase timeout length of 250ms in ticks 
        uint32_t target_time = 3000L / (1000 / CLOCK_RATE);
        start_clock();

        // Work out which device was selected from the ACSI ID of the command byte
        logical_drive_t* selected_device;
        if (acsi_device == logical_drive[0].acsi_id) {
            selected_device = &logical_drive[0];
        } else if (acsi_device == logical_drive[1].acsi_id) {
            selected_device = &logical_drive[1];
        } else {
            // Wrong device selected - should be impossible
            debug_nocr("*** ERROR: Got bad ACSI ID ");
            debug_decimal(acsi_device);
            debug_nocr(" with command ");
            debug_hex(opcode, 2);
            debug("!");

            // Manually unselect device and restart
            extra_data_byte |= 0b01000000;
            write_extra_byte();
            extra_data_byte &= 0b10111111;

            continue;
        }

        debug_nocr("ACSI ID: ");
        debug_decimal(acsi_device);
        debug_nocr(" CMD: ");
        debug_hex(opcode, 2);
        debug("");

        // We are now reading the rest of the command packet
        cmd_length = 0;

        // Don't go beyond the command buffer size
        if (cmd_length < MAX_CMD_LENGTH) {
            cmdbuf[cmd_length] = cmd_byte;
        }
        cmd_length++;

        uint8_t expected_cmd_length = 6;
        
        // Error flag will become true if the command phase times out
        uint8_t error = 0;

        while (cmd_length < expected_cmd_length) {
            cmd_byte = read_command_byte(target_time);

            if (get_clock() >= target_time) {
                debug("Timeout error in command phase");
                error = 1;
                break;
            }

            if (cmd_length < MAX_CMD_LENGTH) {
                cmdbuf[cmd_length] = cmd_byte;
                cmd_length++;
            }

            // After 1 byte, adjust the command length for ICD_ESCAPE 
            // opcode commands
            if (cmd_length == 2) {
                if (opcode == ICD_ESCAPE) {
                    if (cmd_byte < 0x20) {
                        // 6-byte command
                        expected_cmd_length = 7;
                    } else if (cmd_byte < 0x60) {
                        // 10-byte command
                        expected_cmd_length = 11;
                    } else if (cmd_byte < 0xa0) {
                        // 16-byte command
                        expected_cmd_length = 17;
                    } else {
                        // 12-byte command
                        expected_cmd_length = 13;
                    }
                }
            }
        }

        if (error == 0) {

            // Handle command, and do data transfers
            acsi_status_t status;
/*
            if (selected_device->media_changed && opcode != REQUEST_SENSE_6) {
                // If media changed, all commands response with CHECK CONDITION
                // until cleared by a REQUEST SENSE check.
                status = STATUS_CHECK_CONDITION;
            } else {
*/
                switch (opcode) {

                    // Only opcodes in the range 0 - 0x1f are possible here

                    case TEST_UNIT_READY_6:
                        status = acsi_test_unit_ready(selected_device, 0);
                        break;

                    case REQUEST_SENSE_6:
                        // First command issued by ICDTOOLS IDCHECK
                        status = acsi_request_sense(selected_device, 0);
                        break;

                    case FORMAT_UNIT_6:
                        status = acsi_format_unit(selected_device, 0);
                        break;

                    case READ_6:
                        status = acsi_read(selected_device, 0);
                        break;

                    case WRITE_6:
                        status = acsi_write(selected_device, 0);
                        break;

                    // I don't think we really need to support this
                    //case REASSIGN_BLOCKS_6:
                    //    status = acsi_reassign_blocks(selected_device, 0);
                    //    break;

                    case MODE_SELECT_6:
                        status = acsi_mode_select(selected_device, 0);
                        break;

                    case MODE_SENSE_6:
                        status = acsi_mode_sense_6(selected_device, 0);
                        break;

                    case INQUIRY_6:
                        status = scsi_inquiry(selected_device, 0);
                        break;

                    // ICD custom commands
                    case ICD_ESCAPE:
                        status = acsi_icd_escape(selected_device);
                        break;

                    default:
                        selected_device->sense_key = CMD_ERR_INVALID_COMMAND;
                        status = STATUS_CHECK_CONDITION;
                        break;
                }
//            }

            // Send status byte
            set_command_mode();
            set_data_out();
            send_status(status);
        }

        // Manually unselect device and clear buffers
        extra_data_byte |= 0b01000001;
        write_extra_byte();
        extra_data_byte &= 0b10111110;
    }
    
    return 0;
}
