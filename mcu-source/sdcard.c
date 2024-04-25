#include <avr/io.h>

#include "sdcard.h"
#include "timer0.h"
#include "debug.h"
#include "spi.h"

// Card data for each card in the system
// In this case, one internal card, and one front-facing card slot
sdcard_state_t sdcards[2];

void sdcard_setup() {

    // Card 0 CS - output
    SPI_CS0_PORT.DIRSET = SPI_CS0_BIT;

    // Card 1 CS - output
    SPI_CS1_PORT.DIRSET = SPI_CS1_BIT;

    sd_unselect();
}

void sd_select(uint8_t bus_id) {
    // Give SD card time to finish whatever it's doing
    spi_start();
    spi_wait_ready();

	// Chip select ON (low). Use correct output signal for selected card, 0 or 1.
    switch(bus_id) {
        case 0:
            SPI_CS0_PORT.OUTCLR = SPI_CS0_BIT;
            break;
        case 1:
            SPI_CS1_PORT.OUTCLR = SPI_CS1_BIT;
            break;
        default:
            debug_nocr("Non-existent bus ID selected: ");
            debug_decimal(bus_id);
            debug("");
    }

    // Give SD card time to recognise CS has changed
    spi_start();
    spi_wait_ready();
}

void sd_unselect() {
    // Give SD card some clock cycles to finish whatever it's doing
    spi_start();
    spi_wait_ready();

	// Turn both chip selects OFF 
    SPI_CS0_PORT.OUTSET = SPI_CS0_BIT;
    SPI_CS1_PORT.OUTSET = SPI_CS1_BIT;

    // Give SD card time to recognise CS has changed
    spi_start();
    spi_wait_ready();
}

uint8_t CRC7_one(uint8_t crcIn, uint8_t data)
{
	const uint8_t g = 0x89;
	uint8_t i;

	crcIn ^= data;
	for (i = 0; i < 8; i++) {
		if (crcIn & 0x80) crcIn ^= g;
		crcIn <<= 1;
	}

	return crcIn;
}

uint8_t CRC7_buf(uint8_t *pBuf, uint8_t len)
{
	uint8_t crc = 0;
	while (len--) crc = CRC7_one(crc, *pBuf++);
	return crc | _BV(0);
}

void sd_command(uint8_t cmd, uint32_t arg)
{
    uint8_t buf[5];

    if (debug_level > 5) {
        debug_nocr("SD CMD");
        debug_decimal(cmd);
        debug("");
    }
    buf[0] = cmd | SD_CMD_TRANSMISSION_BIT;
    buf[1] = (uint8_t)(arg >> 24);
    buf[2] = (uint8_t)(arg >> 16);
    buf[3] = (uint8_t)(arg >> 8);
    buf[4] = (uint8_t)(arg);

    // Wait for MISO high indicating card is ready to receive
    // FIXME: this doesn't seem to work at least for some cards (e.g. silver Panasonic 4GB)
    //while ((PINB & PIN_MISO) == 0) {
    //}

    // transmit command to sd card with 5 prefix bytes for sync
    spi_transfer(0xff);
    spi_transfer(0xff);
    spi_transfer(0xff);
    spi_transfer(0xff);
    spi_transfer(0xff);

    // Command
    spi_transfer(buf[0]);

    // Argument
    spi_transfer(buf[1]);
    spi_transfer(buf[2]);
    spi_transfer(buf[3]);
    spi_transfer(buf[4]);

    // CRC (optional for SPI mode)
    spi_transfer(CRC7_buf(buf, 5));
}

void debug_r1_response_messages(uint8_t resp) {
    // Bit 0 - In Idle State - The card is in idle state and running the initializing process (this is a normal condition)
    // Bit 1 - Erase Reset - An erase sequence was cleared before executing because an out of erase sequence command was received.
    // Bit 2 - Illegal Command - An illegal command code was detected.
    // Bit 3 - CRC Error - The CRC check of the last command failed.
    // Bit 4 - Erase Sequence Error - An error in the sequence of erase commands occurred.
    // Bit 5 - Address Error - A misaligned address that did not match the block length was used in the command.
    // Bit 6 - Parameter Error - The command’s argument (e.g. address, block length) was outside the allowed range for this card.
    // Bit 7 - Always 0

    if (resp & SD_CMD_R1_ERASE_RESET) {
        debug("  R1 err 'Erase Reset'");
    }
    if (resp & SD_CMD_R1_ILLEGAL_COMMAND) {
        debug("  R1 err 'Illegal Command'");
    }
    if (resp & SD_CMD_R1_CRC_ERROR) {
        debug("  R1 err 'CRC Error");
    }
    if (resp & SD_CMD_R1_ERASE_SEQUENCE_ERROR) {
        debug("  R1 err 'Erase Sequence Command'");
    }
    if (resp & SD_CMD_R1_ADDRESS_ERROR) {
        debug("  R1 err 'Address Error'");
    }
    if (resp & SD_CMD_R1_PARAMETER_ERROR) {
        debug("  R1 err 'Parameter Error'");
    }
}

// R1 is a single-byte response with 7 boolean result flags in it (MSB is always 0)
uint8_t sd_response_r1() {
    uint8_t i = 0, res1;

    // keep polling until data received
    while((res1 = spi_transfer(SD_RESPONSE_WAITING)) & SD_CMD_R1_RESPONSE_MSB_ZERO) {
        // if no respone not received for 8 bytes, break
        if (++i > 8) {
            break;
        }
    }

#if defined(DEBUG_ENABLED)
    if (res1 & _BV(7)) {
        debug("  SD card response R1 timed out");
    } else {
        debug_r1_response_messages(res1);
    }
#endif

    return res1;
}

uint8_t sd_response_r1b() {
    // This generally returns a failure code, but it doesn't seem to matter.

    uint8_t i = 0, res1;

    // keep polling until data received
    while((res1 = spi_transfer(0xff)) & SD_CMD_R1_RESPONSE_MSB_ZERO) {
        // if no respone not received for 8 bytes, break
        if (++i > 8) {
            break;
        }
    }

    return res1;
}

// Response to CMD13 SEND_STATUS
uint8_t sd_response_r2() {
    uint16_t res1 = sd_response_r1();
    if (res1 & _BV(7)) {
        return res1;
    }

    // Get second byte
    uint8_t res2 = spi_transfer(SD_RESPONSE_WAITING);

    if (res2 & SD_CMD_R2_CARD_LOCKED) {
        debug("  R2 err 'Card Locked'");
    }
    if (res2 & SD_CMD_R2_ERASE_SKIP) {
        debug("  R2 err 'Write Protect Erase Skip / Lock/Unlock Command Failed'");
    }
    if (res2 & SD_CMD_R2_ERROR) {
        debug("  R2 err 'General Error'");
    }
    if (res2 & SD_CMD_R2_CC_ERROR) {
        debug("  R2 err 'Card Controller Error'");
    }
    if (res2 & SD_CMD_R2_CARD_ECC_FAILED) {
        debug("  R2 err 'Card ECC Failed'");
    }
    if (res2 & SD_CMD_R2_OUT_OF_RANGE) {
        debug("  R2 err 'Out of Range / CSD Overwrite'");
    }

    res1 <<= 8;
    res1 |= res2;
    return res1;

}

void sd_response_r3(uint8_t *buf) {
    uint8_t i = 0;

    // keep polling until first byte of data received
    while((buf[0] = spi_transfer(0xff)) == 0xff)
    {
        i++;

        // if no data received for 8 bytes, break
        if (i > 8) {
            break;
        }
    }

#if defined(DEBUG_ENABLED)
    if (buf[0] & _BV(7)) {
        debug("  SD card response R3 timed out");
    } else {
        debug_r1_response_messages(buf[0]);
    }
#endif

    // First byte of data is the same as R1's response.
    if (buf[0] & _BV(7)) {
        return;
    }

    // Read the rest of the R3 response, which is 4 more bytes
    buf[1] = spi_transfer(0xff);
    buf[2] = spi_transfer(0xff);
    buf[3] = spi_transfer(0xff);
    buf[4] = spi_transfer(0xff);
}

void sd_response_r7(uint8_t *buf) {
    uint8_t i = 0;

    // keep polling until first byte of data received
    while((buf[0] = spi_transfer(0xff)) == 0xff)
    {
        i++;

        // if no data received for 8 bytes, break
        if (i > 8) {
            break;
        }
    }

#if defined(DEBUG_ENABLED)
    if (buf[0] == 0xff) {
        debug("  SD card response R7 timed out");
    } else {
        debug_r1_response_messages(buf[0]);
    }
#endif

    // First byte of data is the same as R1's response.
    if ((buf[0] & SD_CMD_R1_IN_IDLE_STATE) == 0) {
        return;
    }

    // Read the rest of the R7 response, which is 4 more bytes
    buf[1] = spi_transfer(0xff);
    buf[2] = spi_transfer(0xff);
    buf[3] = spi_transfer(0xff);
    buf[4] = spi_transfer(0xff);

    // I don't know what to use these for, if anything
    uint8_t cmd_version = buf[1] >> 4;
    debug_nocr("R7 command version ");
    debug_decimal(cmd_version);
    debug("");
    uint8_t voltage_accepted = buf[3] & 0xf;
    debug_nocr("R7 indicated voltages ");
    debug_hex(voltage_accepted, 2);
    debug("");
}

uint8_t sd_response() {
    uint8_t i = 0, res1;

    // keep polling until data received
    while((res1 = spi_transfer(0xff)) == 0xff)
    {
        i++;

        // if no data received for 8 bytes, break
        if (i > 8) {
            break;
        }
    }

    return res1;
}

// Wait for SPI response to not be X
uint8_t wait_spi_response(uint8_t ms_delay, uint8_t x) {
    uint8_t result;

    // Clock rate is 100Hz
    uint32_t target_time = ((uint32_t)ms_delay) / (1000 / CLOCK_RATE);

    //debug("- Waiting for SPI transfer");
    start_clock();

    while (1) {
        // The card needs lots of clock cycles to prepare a block for reading, so we must keep toggling the clock
        // line while we wait. Do this 256 bytes at a time.
        uint16_t loops = 256;
        while (loops-- != 0) {
            result = spi_transfer(0xff);

            if (result != x) {
                return result;
            }
        }

        if (get_clock() >= target_time) {
            // A failure
            return result;
        }
    }
}

// Wait for SPI response to be X
uint8_t wait_spi_response2(uint8_t ms_delay, uint8_t x) {
    uint8_t result;

    // Clock rate is 100Hz
    uint32_t target_time = ((uint32_t)ms_delay) / (1000 / CLOCK_RATE);

    //debug("- Waiting for SPI transfer");
    start_clock();

    while (1) {
        // The card needs lots of clock cycles to prepare a block for reading, so we must keep toggling the clock
        // line while we wait. Do this 256 bytes at a time.
        uint16_t loops = 256;
        while (loops-- != 0) {
            result = spi_transfer(0xff);

            if (result == x) {
                return result;
            }
        }

        if (get_clock() >= target_time) {
            // A failure
            return result;
        }
    }
}

// Apply default values to sdcard structure
void sdcard_defaults(sdcard_state_t *sdcard, uint8_t card_id) {
    // Set state to default
    sdcard->bus_id = card_id;
    sdcard->type = SD_CARD_TYPE_SDSC;
    sdcard->capacity = 0;
    sdcard->ccs = 0;
    sdcard->initialised = 0;
    sdcard->usable = 0;
    sdcard->detected = 0;
}

void sdcard_init(sdcard_state_t *sdcard) {

    uint8_t sd_cmd_buf[18];
    uint8_t attempts;
    uint8_t sd_reply;

    debug("SD card init");
    
    // Give the SD card a bunch of clock pulses to practice on and to clear any existing operation.
    sd_select(sdcard->bus_id);
    for (uint16_t i = 0; i < 280; i++) {
        spi_transfer(0xff);
    }
    sd_unselect();

    // Ask the card to wake up and go idle in SPI mode
    attempts = 0;
    while (1) {
        sd_select(sdcard->bus_id);
        sd_command(SD_CMD_0_GO_IDLE_STATE, 0);
        sd_reply = sd_response_r1();
        sd_unselect();

        if (sd_reply == SD_CMD_R1_IN_IDLE_STATE) {
            // Success
            break;
        } else {
            // Not in idle state... yet. We will wait up to 500ms.
            debug("...waiting for idle state");
            if (++attempts > 10) {
                debug("...Failed. Never received a response to idle request.");
                sdcard->initialised = 1;
                sdcard->usable = 0;
                return;
            }
            _delay_ms(50);
        }
    }

    // CMD8 SEND_IF_COND - Setup card. Only returns 1 (error) byte if it's a generation 1 card, or 5 bytes
    // if it's a generation 2 card
    // NOTE card_version will also be set to 0 for a generation 1 card (SDSC) or 1 for a generation 2 card (SDHC/SDXC)
    sd_select(sdcard->bus_id);

    // This 1 bit represents 2.7 - 3.6v operation requested
    uint32_t param = (1 << 8) | SD_CMD_CHECK_PATTERN;
    sd_command(SD_CMD_8_SEND_IF_COND, param);
    sd_response_r7(sd_cmd_buf);
    sd_unselect();

    // We expect to see IN_IDLE_STATE result for SDHC or an error for old SD
    if (sd_cmd_buf[0] == SD_CMD_R1_IN_IDLE_STATE) {
        // This isn't right. Some SDSC cards end up in here.
        //sdcard->type = SD_CARD_TYPE_SDHC;
        //debug("Card is SDHC (ver 2)");

        if (sd_cmd_buf[4] != SD_CMD_CHECK_PATTERN) {
            debug("SD CMD failed check pattern");
            return;
        }
    } else {
        //debug("Card is SD (ver 1)");
        // This is already the default.
        //sdcard->type = SD_CARD_TYPE_SDSC;
    }

    // CMD58 READ_OCR - Check card type
    // "Operation Conditions Register" - 4 bytes
    sd_select(sdcard->bus_id);
    sd_command(SD_CMD_58_READ_OCR, 0);
    sd_response_r3(sd_cmd_buf);
    sd_unselect();

    if (sd_cmd_buf[0] == SD_RESPONSE_R1_CARD_READY 
        || sd_cmd_buf[0] == SD_RESPONSE_R1_IN_IDLE_STATE) {

        // Check voltage range in byte 3
        if (sd_cmd_buf[2] & SD_OCR_VOLTAGE_33_34) {
            debug("3.3v operation OK");
        } else {
            debug("3.3v operation no good. Unusable card.");
            debug_hex(sd_cmd_buf[2], 2);
            debug("");
            return;
        }

        // Check protocol version
        uint8_t command_version = sd_cmd_buf[1] >> 4;
        debug_nocr("Card command version: ");
        debug_decimal(command_version);
        debug("");

        if (sdcard->type == SD_CARD_TYPE_SDHC && (sd_cmd_buf[1] & 0xc0) == 0xc0) {
            sdcard->type = SD_CARD_TYPE_SDXC;
            debug("Card is SDXC");
        }
    } else {
        debug("Failed in CMD58 (1) - card not ready");
        return;
    }

    // Tell the card to initialise itself repeatedly until it returns "CARD READY" or an error.
    // We give 1 second timeout as that amount is given in the spec.
    while (1) {

        sd_select(sdcard->bus_id);
        // CMD55 APP_CMD - Application-specific command
        sd_command(SD_CMD_55_APPLICATION_SPECIFIC_COMMAND, 0);
        sd_reply = sd_response_r1();

        // Possible valid replies: CARD READY (0x00) or IN IDLE STATE (0x01)
        if (sd_reply == SD_RESPONSE_R1_CARD_READY || sd_reply == SD_RESPONSE_R1_IN_IDLE_STATE) {
            // continue on to sending ACMD 41 SEND_OP_COND - Send operating condition

            sd_command(SD_APP_CMD_41_SEND_OP_COND, SD_ACMD41_HIGH_CAPACITY_SUPPORT);
            sd_reply = sd_response_r1();
        }

        sd_unselect();

        if (sd_reply == SD_RESPONSE_R1_CARD_READY) {
            // Succeeded
            break;
        } else if (sd_reply == SD_RESPONSE_R1_IN_IDLE_STATE) {
            // Still waiting
            if (++attempts > 100) {
                debug("Failed to initialise SD card.");
                return;
            }
            _delay_ms(10);
            continue;
        }

        // Error response from card
        sdcard->initialised = 1;
        debug("Failed.");
        return;
    }

    // SDSD cards (ver1) are finished initialising now. Version 2+ cards have more to do.
    if (sdcard->type != SD_CARD_TYPE_SDSC) {

        // CMD58 READ_OCR - Check card type (again)
        // This time we are interested in the CCS bit which should be now valid.
        sd_select(sdcard->bus_id);
        sd_command(SD_CMD_58_READ_OCR, 0);
        sd_response_r3(sd_cmd_buf);
        sd_unselect();

        if (sd_cmd_buf[0] == SD_RESPONSE_R1_CARD_READY) {
            // Card capacity status 0=SDSD, 1=SDHC/SDXC
            // Check power-up status bit is set first
            if (sd_cmd_buf[1] & _BV(7)) {
                // Then check if CCS bit is set
                if (sd_cmd_buf[1] & _BV(6)) {
                    // This determines whether addresses will be in byte or sector units
                    sdcard->ccs = 1;
                    debug("SD Card version 2 - high-capacity card");
                } else {
                    debug("SD Card version 2 - standard-capacity card");
                }
            }
        } else {
            debug("Failed in CMD58 (2) - card not ready.");
            return;
        }
    }

    // CMD9 SEND_CSD - To find out the card size
    debug("Getting CSD");
    sd_select(sdcard->bus_id);
    sd_command(SD_CMD_9_SEND_CSD, 0);
    sd_reply = sd_response_r1();
    if (sd_reply == SD_RESPONSE_R1_CARD_READY) {

        // Wait for response to start
        sd_cmd_buf[0] = wait_spi_response(100, 0xff);

        if (sd_cmd_buf[0] == 0xfe) {
            // It's gotta be 0xfe!
        
            // Get the full CSR register (16 bytes)
            for (uint8_t count = 0; count < 16; count++) {
                sd_cmd_buf[count] = spi_transfer(0xff);
            }

            // Get, and ignore, 2 byte CRC
            spi_transfer(0xff);
            spi_transfer(0xff);

            // Provide 8 additional clocks to make the SD card happy
            spi_transfer(0xff);

            // Get CSD
            card_version = sd_cmd_buf[0] >> 6;

            if (card_version == 0) {
                // This is the silly way to do things
                debug("SD version 1");
                sdcard->type = SD_CARD_TYPE_SDSC;

                // READ_BLOCK_LEN bits 83:80 (4 bits)
                uint8_t read_bl_len = sd_cmd_buf[5] & 0xf; // bits 83-80
                uint32_t card_block_size = 1 << read_bl_len;
                // E.g. value of 9 comes to 512

                // C_SIZE bits 73:62 (12 bits)
                uint32_t c_size = sd_cmd_buf[6] & 3; // bits 73:72
                c_size <<= 8;
                c_size |= sd_cmd_buf[7]; // bits 71:64
                c_size <<= 2;
                c_size |= sd_cmd_buf[8] >> 6; // bits 63:62

                // BLOCK_SIZE bits 49:47 (3 bits)
                uint8_t c_size_mult = sd_cmd_buf[9] & 0x3; // bits 49:48
                c_size_mult <<= 1;
                c_size_mult |= sd_cmd_buf[10] >> 7; // bit 47

                uint8_t read_bl_partial = (sd_cmd_buf[10] >> 6) & 1;
                debug_nocr("READ_BL_PARTIAL: ");
                debug_decimal(read_bl_partial);
                debug("");

                uint8_t write_bl_len = sd_cmd_buf[12] & 0x3; // bits 25-24
                write_bl_len <<= 2;
                write_bl_len |= (sd_cmd_buf[13] >> 6) & 0x3; // bits 22-23
                debug_nocr("WRITE_BL_LEN: ");
                debug_decimal(write_bl_len);
                debug("");

                // Weird maths
                sdcard->capacity = c_size + 1;
                sdcard->capacity *= (1 << (c_size_mult + 2));
                sdcard->capacity *= card_block_size;
                sdcard->capacity /= 512;

                // FIXME: how does this work
                /*
                if (card_block_size != 512) {
                    // TODO
                    // Unsuitable card, chuck an error here
                    debug_nocr("SD card unsuitable, bad block size: ");
                    debug_decimal(card_block_size);
                    debug("");
                    sd_unselect();
                    return;
                }*/
            } else {
                // The sane way to do things - for CSD version 2.0 (SDHC and SDXC)
                debug("SD version 2");
                sdcard->type = SD_CARD_TYPE_SDHC; // FIXME: or SDXC, SDUC

                // C_SIZE vits 69:48 (22 bits)
                uint32_t blocks = sd_cmd_buf[7] & 0x3f; // bits 69:64
                blocks <<= 8;
                blocks |= sd_cmd_buf[8];
                blocks <<= 8;
                blocks |= sd_cmd_buf[9];
                blocks += 1;

                // C_SIZE is in units of 1024 blocks (512KB)
                sdcard->capacity = blocks * 1024;
            }
        } else {
            debug("*** ERR wrong response reading SD CSD");
            sdcard->capacity = 0;
            sd_unselect();
            return;
        }
    } else {
        debug("SD CMD9 failed");
        sdcard->capacity = 0;
        sd_unselect();
        return;
    }

    sd_unselect();

    if (debug_level > 0) {
        debug_nocr("=== SD Card ");
        debug_decimal(sdcard->bus_id);
        debug("");
        debug_nocr("- Type: ");
        if (sdcard->type == SD_CARD_TYPE_SDSC) {
            debug("SDSC");
        } else if (sdcard->type == SD_CARD_TYPE_SDHC) {
            debug("SDHC");
        } else if (sdcard->type == SD_CARD_TYPE_SDXC) {
            debug("SDXC");
        } else {
            debug("SDUC");
        }

        debug_nocr("- Capacity: ");
        debug_decimal(sdcard->capacity / 2);
        debug("KiB");

        debug("===");
    }

    debug("SD card finished init");
    sdcard->initialised = 1;
    sdcard->usable = 1;
    return;
}
