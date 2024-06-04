#include "acsi.h"
#include "fpga_comm.h"
#include "sdcard.h"
#include "modesense.h"
#include "debug.h"
#include "commands.h"
#include "spi.h"
#include "rtc.h"

// The two logical drives that are present on this device
logical_drive_t logical_drive[2];

// Test Unit Ready: just returns an OK status to check that the device is there.
acsi_status_t acsi_test_unit_ready(logical_drive_t *device, uint8_t cmd_offset) {
    return STATUS_OK;
}

// Request Sense: returns device status information, most importantly the last reported error.
acsi_status_t acsi_request_sense(logical_drive_t *device, uint8_t cmd_offset) {
    set_data_mode();

/*
    if (device->media_changed) {
        device->media_changed = 0;

        write_byte(6); // UNIT ATTENTION
        write_byte(0x28); // not-ready to ready transition
        write_byte(3); // media changed
        write_byte(0);

    } else {
*/
        write_byte(device->sense_key);
        write_byte(0); // TODO
        write_byte(0);
        write_byte(0);
//    }

    device->sense_key = DRV_ERR_NO_ERROR;
    return STATUS_OK;
}

// Format Unit (unused): low-level format for a disk drive. This was used on magnetic media.
acsi_status_t acsi_format_unit(logical_drive_t *device, uint8_t cmd_offset) {
    // Let's not and say we did!
    debug("FORMAT UNIT (6) - dummy operation");

    return STATUS_OK;
}

// Read: read a number of blocks at a given address
#define MULTI_BLOCK_READ
//#define SD_INTERRUPTS
#define DOUBLE_BUFFERED_MODE

// Read one block from SD card and send via ACSI
acsi_status_t read_block(logical_drive_t *device, uint32_t addr) {
    sdcard_state_t *sdcard = device->sdcard;
    
    // Calculate adjusted address
    uint32_t sd_addr = addr;
    if (sdcard->type == SD_CARD_TYPE_SDSC) {
        sd_addr *= 512;
    }

    sd_select(sdcard->bus_id);
    sd_command(SD_CMD_17_BLOCK_READ, sd_addr);
    uint8_t result = sd_response_r1();

    // If a reply was received
    if (result != 0xff) {

        // Wait for card to be ready to send data. It will send 0xff while it's preparing
        result = wait_spi_response(100, 0xff);
        if (result == 0xff) {
            sd_unselect();
            green_led_off();
            debug_nocr("*** ERR SD timeout reading blk ");
            debug_hex(addr, 8);
            debug("");
            device->sense_key = CNT_ERR_DATA_ERROR;
            return STATUS_CHECK_CONDITION;
        }            

        if (result == SD_CMD_BLOCK_READY_TOKEN) {
            // Go!
            set_data_out();

#if defined(SD_INTERRUPTS)
            sdcard_read_sector_to_buffer(&sd_buffer);
            while (sd_buffer.done == 0) {
                    debug_nocr("Waiting - count=");
                    debug_decimal(sd_buffer.count);
                    debug_nocr(" tx_count=");
                    debug_decimal(sd_buffer.tx_count);
                    debug("");

            }
            // Read the unused CRC field
            spi_transfer(0xff);
            spi_transfer(0xff);

#else
            // Switch to buffered SPI mode (it's already got a 4-byte buffer so no need)
            //SPI.CTRLA &= ~SPI_ENABLE_bm;
            //SPI.CTRLB |= SPI_BUFEN_bm;
            //SPI.CTRLA |= SPI_ENABLE_bm;

            // Start 2-way SPI data transmission from the SD card by writing two dummy values
            spi_start();
            spi_start();

            for (uint16_t byte = 0; byte < 510; byte++) {
                //uint8_t value = spi_in_nowait();
                // Then send the value
                write_byte_nochecks(spi_in_nowait());
                // Keep the SPI transfer going for the next byte
                spi_start();
            }
            // Do the 511th and 512th byte without sending
            write_byte_nochecks(spi_in_nowait());
            write_byte_nochecks(spi_in_nowait());

            // disable buffered mode
            //SPI.CTRLA &= ~SPI_ENABLE_bm;
            //SPI.CTRLB &= ~SPI_BUFEN_bm;
            //SPI.CTRLA |= SPI_ENABLE_bm;
            
            // Read the unused CRC field (we don't have time to calculate this)
            spi_transfer(0xff);
            spi_transfer(0xff);
#endif
        }
    } else {
        debug_nocr("*** ERR invalid resp from CMD17 reading blk ");
        debug_hex(addr, 8);
        debug("");
        device->sense_key = CNT_ERR_DATA_ERROR;
        return STATUS_CHECK_CONDITION;
    }
    sd_unselect();
    return STATUS_OK;
}

static inline void transfer_1_byte() {
    write_byte_nochecks(spi_in_nowait());
    // Keep the SPI transfer going for the next byte
    spi_start();
}

static inline void transfer_16_bytes() {
    transfer_1_byte();
    transfer_1_byte();
    transfer_1_byte();
    transfer_1_byte();
    transfer_1_byte();
    transfer_1_byte();
    transfer_1_byte();
    transfer_1_byte();
    transfer_1_byte();
    transfer_1_byte();
    transfer_1_byte();
    transfer_1_byte();
    transfer_1_byte();
    transfer_1_byte();
    transfer_1_byte();
    transfer_1_byte();
}

static inline void transfer_256_bytes() {
    transfer_16_bytes();
    transfer_16_bytes();
    transfer_16_bytes();
    transfer_16_bytes();
    transfer_16_bytes();
    transfer_16_bytes();
    transfer_16_bytes();
    transfer_16_bytes();
    transfer_16_bytes();
    transfer_16_bytes();
    transfer_16_bytes();
    transfer_16_bytes();
    transfer_16_bytes();
    transfer_16_bytes();
    transfer_16_bytes();
    transfer_16_bytes();
}

acsi_status_t acsi_read(logical_drive_t *device, uint8_t cmd_offset) {
    sdcard_state_t *sdcard = device->sdcard;

    uint32_t addr;
    uint16_t transfer_length;
    acsi_status_t status = STATUS_OK;

    // Source block address
    //addr = cmdbuf[cmd_offset + 1] & 1f;  // Most-significant byte is 5 bits for SCSI, 8 bits for ACSI
    addr = cmdbuf[cmd_offset + 1];
    addr <<= 8;
    addr |= cmdbuf[cmd_offset + 2];
    addr <<= 8;
    addr |= cmdbuf[cmd_offset + 3];

    // Transfer length in blocks, 1 to 255 or 0=256
    transfer_length = cmdbuf[cmd_offset + 4];
    // SCSI - but not ACSI:
    //if (transfer_length == 0) {
    //    transfer_length = 256;
    //}

    debug_nocr("READ (6) ");
    debug_decimal(transfer_length);
    debug_nocr(" blocks from sector ");
    debug_hex(addr, 8);
    debug("");

    if ((addr + transfer_length - 1) >= sdcard->capacity) {
        device->sense_key = CMD_ERR_INVALID_ADDRESS;
        debug("Sector not found");
        return STATUS_CHECK_CONDITION;
    }

    green_led_on();
    set_data_out();
    set_data_mode();

#ifdef MULTI_BLOCK_READ

    uint32_t sd_addr = addr;
    if (sdcard->type == SD_CARD_TYPE_SDSC) {
        sd_addr *= 512;
    }

    sd_select(sdcard->bus_id);
    sd_command(SD_CMD_18_MULTIPLE_BLOCK_READ, sd_addr);
    uint8_t result = sd_response_r1();
    if (result != 0xff) {
        // Card responded

#ifdef DOUBLE_BUFFERED_MODE
        // Switch to double-buffered data mode
        extra_data_byte |= 0b00000010;
        write_extra_byte();
#endif

        for (uint16_t i = 0; i < transfer_length; i++) {
            // Wait for block ready token.
            result = wait_spi_response(100, 0xff);
            if (result == 0xff) {
                sd_unselect();
                green_led_off();
                debug_nocr("*** ERR SD timeout reading blk ");
                debug_hex(addr, 8);
                debug("");
                device->sense_key = CNT_ERR_DATA_ERROR;
                return STATUS_CHECK_CONDITION;
            }

            if (result == SD_CMD_BLOCK_READY_TOKEN) {
                // Go!
                //debug("Got block ready token");


#if defined(SD_INTERRUPTS)
                sdcard_read_sector_to_buffer(&sd_buffer);
                while (sd_buffer.done == 0) {
                    debug_nocr("Waiting - count=");
                    debug_decimal(sd_buffer.count);
                    debug_nocr(" tx_count=");
                    debug_decimal(sd_buffer.tx_count);
                    debug("");
                }
                // Read the unused CRC field
                spi_transfer(0xff);
                spi_transfer(0xff);

#else
                // Switch to buffered SPI mode
                //SPI.CTRLA &= ~SPI_ENABLE_bm;
                //SPI.CTRLB |= SPI_BUFEN_bm;
                //SPI.CTRLA |= SPI_ENABLE_bm;

                // Start 2-way SPI data transmission from the SD card by writing two dummy values
                // This prefills the 2-byte FIFO
                spi_start();
                spi_start();
/*
                for (uint16_t byte = 0; byte < 510; byte++) {
                    //uint8_t value = spi_in_nowait();
                    // Then send the value
                    write_byte_nochecks(spi_in_nowait());
                    // Keep the SPI transfer going for the next byte
                    spi_start();
                }*/
                transfer_256_bytes();
                transfer_16_bytes();
                transfer_16_bytes();
                transfer_16_bytes();
                transfer_16_bytes();
                transfer_16_bytes();
                transfer_16_bytes();
                transfer_16_bytes();
                transfer_16_bytes();
                transfer_16_bytes();
                transfer_16_bytes();
                transfer_16_bytes();
                transfer_16_bytes();
                transfer_16_bytes();
                transfer_16_bytes();
                transfer_16_bytes();
                transfer_1_byte();
                transfer_1_byte();
                transfer_1_byte();
                transfer_1_byte();
                transfer_1_byte();
                transfer_1_byte();
                transfer_1_byte();
                transfer_1_byte();
                transfer_1_byte();
                transfer_1_byte();
                transfer_1_byte();
                transfer_1_byte();
                transfer_1_byte();
                transfer_1_byte();
                // Do the 511th and 512th byte without sending
                write_byte_nochecks(spi_in_nowait());
                write_byte_nochecks(spi_in_nowait());

                // disable buffered mode
                //SPI.CTRLA &= ~SPI_ENABLE_bm;
                //SPI.CTRLB &= ~SPI_BUFEN_bm;
                //SPI.CTRLA |= SPI_ENABLE_bm;
                
                // Read the unused CRC field (we don't have time to calculate this)
                spi_transfer(0xff);
                spi_transfer(0xff);
#endif
                //debug("Fin xfer from card");
            }            
        }

#ifdef DOUBLE_BUFFERED_MODE
        // Switch off double-buffered data mode and clear the buffers
        extra_data_byte |= 0b00000001;
        write_extra_byte();

        extra_data_byte &= 0b11111100;
        write_extra_byte();
#endif        

        // Send the Stop Transfer command
        sd_command(SD_CMD_12_STOP_TRANSMISSION, 0);

        // Throw away one byte to give the card time to stop
        spi_transfer(0xff);

        // Now the real result
        result = sd_response_r1b();
        //debug_nocr("got R1b response ");
        //debug_hex(result);
        //debug("");
        /*
        // Nobody else out there seems to check this result.
        if (result != SD_CMD_R1_IN_IDLE_STATE) {
            debug_nocr("*** ERR invalid resp from CMD12: ");
            debug_hex(result);
            debug("");
            status = STATUS_CHECK_CONDITION;
            sense_key = CNT_ERR_DATA_ERROR;
        }
*/
        // Skip any "busy" zeroes
        while (spi_transfer(0xff) != 0xff) {
            // Card holds D0 low while busy
        }

    } else {
        debug_nocr("*** ERR invalid resp from CMD18 reading blk ");
        debug_hex(addr, 8);
        debug("");
        status = STATUS_CHECK_CONDITION;
        device->sense_key = CNT_ERR_DATA_ERROR;
    }
    sd_unselect();

#else

    for (uint16_t i = 0; i < transfer_length; i++) {
        status = read_block(device, addr + i);
        if (status != STATUS_OK) {
            break;
        }
    }
#endif

    green_led_off();
    return status;
}

acsi_status_t read_10(logical_drive_t *device, uint8_t cmd_offset) {
    sdcard_state_t *sdcard = device->sdcard;

    uint32_t addr;
    uint16_t transfer_length;
    acsi_status_t status = STATUS_OK;

    // Source block address
    addr = cmdbuf[cmd_offset + 2];
    addr <<= 8;
    addr |= cmdbuf[cmd_offset + 3];
    addr <<= 8;
    addr |= cmdbuf[cmd_offset + 4];
    addr <<= 8;
    addr |= cmdbuf[cmd_offset + 5];

    // Transfer length in blocks, 1 to 255 or 0=256
    transfer_length = cmdbuf[cmd_offset + 7];
    transfer_length <<= 8;
    transfer_length |= cmdbuf[cmd_offset + 8];

    if (transfer_length == 0) {
        transfer_length = 256;
    }

    debug_nocr("READ (10) ");
    debug_decimal(transfer_length);
    debug_nocr(" blocks from sector ");
    debug_hex(addr, 8);
    debug("");

    if ((addr + transfer_length - 1) >= sdcard->capacity) {
        device->sense_key = CMD_ERR_INVALID_ADDRESS;
        debug("Sector not found");
        return STATUS_CHECK_CONDITION;
    }

    green_led_on();
    set_data_out();
    set_data_mode();

    for (uint16_t i = 0; i < transfer_length; i++) {
        status = read_block(device, addr + i);
        if (status != STATUS_OK) {
            break;
        }
    }

    green_led_off();
    return status;
}


acsi_status_t sd_card_single_block_write(logical_drive_t *device, uint32_t addr) {
    sdcard_state_t *sdcard = device->sdcard;
    uint8_t result;

    sd_select(sdcard->bus_id);

    sd_command(SD_CMD_24_WRITE_BLOCK, addr);
    result = sd_response_r1();
    if (result != SD_RESPONSE_R1_CARD_READY) {
        sd_unselect();
        device->sense_key = DRV_ERR_WRITE_FAULT;
        return STATUS_CHECK_CONDITION;
    }

    set_data_mode();
    set_data_in();

    // Start block token
    spi_transfer(SD_CMD_SINGLE_BLOCK_START_TOKEN);

    // Get first byte from FPGA
    uint8_t value = read_byte_nochecks();
    spi_start_with_value(value);

    // Send bytes 2-512
    for (uint16_t byte = 1; byte < 512; byte++) {
        // Get next byte from FPGA
        value = read_byte_nochecks();
        // Write to SPI without waiting to read back
        spi_out_nowait(value);
    }
    // Wait for last byte to finish
    spi_wait_ready();

    // Send unused CRC field
    spi_transfer(0xff);
    spi_transfer(0xff);

    // Wait for the card to send a response token
    result = wait_spi_response(100, 0xff);

    if ((result & 0x1f) != SD_CMD_BLOCK_DATA_ACCEPTED) {
        // Data not accepted?
        sd_unselect();

        debug_nocr("*** ERR SD data not accepted, reason: ");
        debug_hex(result, 2);
        debug("");

        device->sense_key = CNT_ERR_DATA_ERROR;
        return STATUS_CHECK_CONDITION;
    } 

    // Wait for data to be written
    result = wait_spi_response2(250, 0xff);
    if (result != 0xff) {
        sd_unselect();

        debug_nocr("*** ERR SD card timeout writing blk ");
        debug_hex(addr, 2);
        debug("");

        device->sense_key = CNT_ERR_DATA_ERROR;
        return STATUS_CHECK_CONDITION;
    }

    sd_command(SD_CMD_13_SEND_STATUS, 0);
    uint16_t r2_result = sd_response_r2();
    if ((r2_result | 0x0100) != SD_RESPONSE_R2_NO_ERROR) {
        debug_nocr("*** R2 reply failed: ");
        debug_hex(r2_result, 2);
        debug("");
        sd_unselect();
        device->sense_key = DRV_ERR_WRITE_FAULT;
        return STATUS_CHECK_CONDITION;
    }

    sd_unselect();

    return STATUS_OK;
}


// Write: write a number of blocks at a given address
acsi_status_t acsi_write(logical_drive_t *device, uint8_t cmd_offset) {

    sdcard_state_t *sdcard = device->sdcard;
    uint8_t result;

    uint32_t addr = cmdbuf[1] & 0x1f;
    addr <<= 8;
    addr |= cmdbuf[2];
    addr <<= 8;
    addr |= cmdbuf[3];
    uint16_t transfer_length = cmdbuf[4];

    if (transfer_length == 0) {
        transfer_length = 256;
    }

    debug_nocr("WRITE (6) ");
    debug_decimal(transfer_length);
    debug_nocr(" blocks to sector ");
    debug_hex(addr, 8);
    debug("");

    // Generation 1 card: address is in bytes, so multiply block number by sector size to get address
    // Generation 2 card: address is in sectors, no need to do anything
    uint32_t sd_addr = addr;
    if (sdcard->type == SD_CARD_TYPE_SDSC) {
        sd_addr *= 512;
    }

    if ((addr + transfer_length - 1) >= sdcard->capacity) {
        device->sense_key = CMD_ERR_INVALID_ADDRESS;
        debug("Sector not found");
        return STATUS_CHECK_CONDITION;
    }

    red_led_on();

    if (transfer_length == 1) {
        acsi_status_t status = sd_card_single_block_write(device, sd_addr);
        red_led_off();
        return status;
    }

    // SD card multi-block pre-erase
    sd_select(sdcard->bus_id);
    sd_command(SD_CMD_55_APPLICATION_SPECIFIC_COMMAND, 0);
    result = sd_response();
    if (result == 0) {
        sd_command(SD_CMD_23_MULTIPLE_BLOCKS_COUNT, transfer_length);
        result = sd_response();
        if (result) {
            sd_unselect();
            red_led_off();

            debug_nocr("*** ERR SD invalid resp to CMD 23 writing blk ");
            debug_hex(addr, 8);
            debug("");

            device->sense_key = CNT_ERR_DATA_ERROR;
            return STATUS_CHECK_CONDITION;
        }
    } else {
        debug_nocr("*** ERR SD invalid resp to CMD 55 writing blk ");
        debug_hex(addr, 8);
        debug("");
        sd_unselect();
        red_led_off();

        device->sense_key = CNT_ERR_DATA_ERROR;
        return STATUS_CHECK_CONDITION;
    }

    // Multi-block write
    sd_command(SD_CMD_25_MULTIPLE_BLOCK_WRITE, sd_addr);
    result = sd_response_r1();
    if (result != 0) {
        sd_unselect();
        red_led_off();
        debug_nocr("*** ERR SD invalid resp to CMD 25 writing blk ");
        debug_hex(addr, 8);
        debug("");
        device->sense_key = CNT_ERR_DATA_ERROR;
        return STATUS_CHECK_CONDITION;
    } else {
        // Skip one byte
        spi_transfer(0xff);

        set_data_mode();
        set_data_in();

        for (uint16_t i = 0; i < transfer_length; i++) {
            spi_transfer(SD_CMD_BLOCK_SEND_TOKEN);

            // Get first byte from FPGA
            uint8_t value = read_byte_nochecks();
            spi_start_with_value(value);

            // Send bytes 2-512
            for (uint16_t byte = 1; byte < 512; byte++) {
                // Get next byte from FPGA
                value = read_byte_nochecks();
                // Write to SPI without waiting to read back
                spi_out_nowait(value);
            }
            // Wait for last byte to finish
            spi_wait_ready(); // TODO I don't think we need this

            // Send unused CRC field
            spi_transfer(0xff);
            spi_transfer(0xff);

            // Wait for the card to send a response token
            result = wait_spi_response(100, 0xff);

            if ((result & 0x1e) != 4) {
                // Data not accepted?
                sd_unselect();
                red_led_off();

                debug_nocr("*** ERR SD data not accepted, reason: ");
                debug_hex(result, 2);
                debug("");

                device->sense_key = CNT_ERR_DATA_ERROR;
                return STATUS_CHECK_CONDITION;
            } 

            // Wait for data to be written
            result = wait_spi_response2(100, 0xff);
            if (result != 0xff) {
                sd_unselect();
                red_led_off();

                debug_nocr("*** ERR SD card timeout writing blk ");
                debug_hex(addr, 8);
                debug("");

                device->sense_key = CNT_ERR_DATA_ERROR;
                return STATUS_CHECK_CONDITION;
            }
        }

        spi_transfer(SD_CMD_STOP_TRAN_TOKEN);
    
        // Skip one byte
        spi_transfer(0xff);

        result = wait_spi_response2(100, 0xff);
        if (result != 0xff) {
            sd_unselect();
            red_led_off();

            debug("*** ERR SD card timeout after multi-block write");

            device->sense_key = CNT_ERR_DATA_ERROR;
            return STATUS_CHECK_CONDITION;
        }
    }
    
    sd_unselect();
    red_led_off();
    return STATUS_OK;
}

/*
// Reassign Blocks: I can't remember what the purpose of this is
acsi_status_t acsi_reassign_blocks(logical_drive_t *device, uint8_t cmd_offset) {
  set_data_mode();

  uint32_t parameter_list_length = read_byte();
  parameter_list_length <<= 8;
  parameter_list_length |= read_byte();
  parameter_list_length <<= 8;
  parameter_list_length |= read_byte();
  parameter_list_length <<= 8;
  parameter_list_length |= read_byte();

  while (parameter_list_length--) {
      // A 4-byte value or FIXME an 8-byte value depending on the command
      read_byte();
      read_byte();
      read_byte();
      read_byte();
  }

  return STATUS_OK;
}
*/

// Mode Select: not sure either. Something to do with LUNs
acsi_status_t acsi_mode_select(logical_drive_t *device, uint8_t cmd_offset) {

    debug("MODE_SELECT (6) - dummy operation");

    // We receive 12 bytes
    set_data_mode();

    for (uint8_t i = 0; i < 12; i++) {
        read_byte();
    }

    // bytes 0-2 unknown
    // byte 3 - set to 8 when 512-byte block is requested, not sure why
    // bytes 10-11 - block size
    return STATUS_OK;
}

// MODE SENSE (6)
acsi_status_t acsi_mode_sense_6(logical_drive_t *device, uint8_t cmd_offset) {
  //uint8_t dbd = cmdbuf[cmd_offset + 1] & 8;
  uint8_t pc = cmdbuf[cmd_offset + 2] >> 6;
  uint8_t page_code = cmdbuf[cmd_offset + 2] & 0x3f;
  //uint8_t subpage_code = cmdbuf[cmd_offset + 3];
  debug_nocr("MODE SENSE (6) pc ");
  debug_decimal(pc);
  debug(" page ");
  debug_decimal(page_code);
  debug("");

  // TODO implement 0 and 1?
  if (pc == 0 && page_code == 0) {
      do_mode_sense_page_0();
  } else if (pc == 0 && page_code == 1) {
      do_mode_sense_page_1();
  } else if (pc == 0 && page_code == 3) {
      do_mode_sense_page_3();
  } else if (pc == 0 && page_code == 4) {
      do_mode_sense_page_4();
  } else if (pc == 0 && page_code == 0x3f) {
      // Everything
      do_mode_sense_page_0();
      do_mode_sense_page_1();
      do_mode_sense_page_3();
      do_mode_sense_page_4();
  } else {
      // These are not all handled yet.
      device->sense_key = CMD_ERR_INVALID_ARGUMENT;
      return STATUS_CHECK_CONDITION;
  }                   

  return STATUS_OK;
}

const uint8_t inquiry_scsi_response[36] = {
    0,                                                      // Peripheral qualifier: device connected
                                                            // Peripheral type: block device
    0x80,                                                   // Removable: yes
    0,                                                      // Version: no claims
    2,                                                      // Response data format: no heirarchical LUN support, response data format 2
    31,                                                     // Additional length: 31 bytes (ends after product revision level)
    0,                                                      // SCCS 0: No embedded storage array controller
                                                            // ACC 0: no access controls coordinator
                                                            // TPGS 0: no target port group support
                                                            // 3PC 0: no third-party copy protection
                                                            // PROTECT 0: no protection support
    0,                                                      // ENCSERV 0: No enclosure support services
                                                            // MULTIP 0: no multi-port support;
    0,                                                      // CMDQUE 0: no command queuing support
    'S', 'm', 'o', 'n', 's', 'o', 'n', ' ',                 // Vendor identification string, 8 bytes
    'D', 'i', 's', 'k', ' ', ' ', ' ', ' ',                 // Product identification string, 16 bytes
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    '0', '0', '0', '5',                                     // Product revision level string, 4 bytes
};

acsi_status_t scsi_inquiry(logical_drive_t *device, uint8_t cmd_offset) {

    uint8_t evpd = cmdbuf[cmd_offset + 1] & 1;
    uint8_t page = cmdbuf[cmd_offset + 2];
    uint16_t allocation_length = cmdbuf[cmd_offset + 3];
    allocation_length <<= 8;
    allocation_length |= cmdbuf[cmd_offset + 4];

    debug_nocr("INQUIRY(6) - page ");
    debug_decimal(page);
    debug_nocr(" evpd ");
    debug_decimal(evpd);
    debug_nocr(" len ");
    debug_decimal(allocation_length);
    debug(" bytes");

    if (allocation_length < sizeof(inquiry_scsi_response)) {
        // Standard response data is (a minimum of) 36 bytes per SCSI spec.
        // If the host can't hold it, return an error.
        device->sense_key = CMD_ERR_INVALID_ARGUMENT;
        return STATUS_CHECK_CONDITION;
    }

    if (evpd == 0 && page != 0) {
        device->sense_key = CMD_ERR_INVALID_ARGUMENT;
        return STATUS_CHECK_CONDITION;
    }

    set_data_mode();
    set_data_out();

    if (evpd == 0) {
        // Copy standard inquiry data (36 bytes) to RAM
        uint8_t buf[sizeof(inquiry_scsi_response)];
        uint8_t i;
        for (i = 0; i < sizeof(inquiry_scsi_response); i++) {
            buf[i] = inquiry_scsi_response[i];
        }

        // Copy the disk size into the description
        uint64_t size = device->sdcard->capacity;
        size *= 512;
        size /= 1000000;
        uint8_t buffer[8];
        uint8_t digit = 0;
        uint8_t pos = 5;
        uint8_t gb = size > 1000;

        if (gb) {
            size /= 1000;
        }
        if (size > 0) {
            while (size > 0) {
                buffer[digit] = '0' + (size % 10);
                size /= 10;
                digit++;
            }
            while (digit > 0) {
                digit--;
                buf[16 + pos++] = buffer[digit];
            }
        } else {
            buf[16 + pos++] = '0';
        }

        buf[16 + pos++] = gb ? 'G' : 'M';
        buf[16 + pos] = 'B';

        // Transfer the standard inquiry data
        for (i = 0; i < sizeof(inquiry_scsi_response); i++) {
            write_byte(buf[i]);
        }
    }
    
    // Don't think anyone is going to care about additional inquiry pages
    /* else if (page == 0x0) {
        // Supported VPD Pages
        write_byte(0); // Peripheral qualifier/device type
        write_byte(0x0); // Page code = 0
        write_byte(0); // Reserved
        write_byte(2); // Page length
        write_byte(0); // Page 0 supported
        write_byte(0xb0); // Page 0xb0 - block limits - supported
    } else if (page == 0xb0) {
        // Block limits page
        green_led_on();

        write_byte(0); // Peripheral qualifier/device type
        write_byte(0xb0); // Page code = 0xb0
        write_byte(0x00); // Page length upper byte
        write_byte(0x3c); // Page length lower byte
        for (int i = 0; i < 62; i++) {
            write_byte(0); // Leave at default
        }
    }
    */
    else {
        device->sense_key = CMD_ERR_INVALID_ARGUMENT;
        return STATUS_CHECK_CONDITION;
    }

    return STATUS_OK;
}

acsi_status_t mode_sense_10(logical_drive_t *device, uint8_t cmd_offset) {
    uint8_t pc = cmdbuf[cmd_offset + 2] >> 6;
    uint8_t page_code = cmdbuf[cmd_offset + 2] & 0x3f;

    debug_nocr("MODE SENSE (10) pc ");
    debug_decimal(pc);
    debug(" page ");
    debug_decimal(page_code);
    debug("");
    if (pc == 0 && page_code == 0) {
        do_mode_sense_page_0();
    } else if (pc == 0 && page_code == 1) {
        do_mode_sense_page_1();
    } else if (pc == 0 && page_code == 3) {
        do_mode_sense_page_3();
    } else if (pc == 0 && page_code == 4) {
        do_mode_sense_page_4();
    } else if (pc == 0 && page_code == 0x3f) {
        // Everything
        do_mode_sense_page_0();
        do_mode_sense_page_1();
        do_mode_sense_page_3();
        do_mode_sense_page_4();
    } else {
        // These are not all handled yet.
        device->sense_key = CMD_ERR_INVALID_ARGUMENT;
        return STATUS_CHECK_CONDITION;
    }

    return STATUS_OK;
}

acsi_status_t read_capacity_10(logical_drive_t *device, uint8_t cmd_offset) {

    uint32_t capacity = device->sdcard->capacity - 1;

    debug_nocr("READ CAPACITY (10) returning ");
    debug_decimal(capacity);
    debug(" blocks");

    set_data_mode();
    set_data_out();

    write_byte(capacity >> 24); // Device capacity (MSB)
    write_byte((capacity >> 16) & 0xff);
    write_byte((capacity >> 8) & 0xff);
    write_byte(capacity & 0xff);

    // 512 bytes
    write_byte(0x0); // Block length (MSB)
    write_byte(0x0); // 
    write_byte(0x2); // 
    write_byte(0x0); //

    return STATUS_OK;
}

acsi_status_t ultrasatan_command(logical_drive_t *device, uint8_t cmd_offset) {
    // TODO: set clock, read clock
    device->sense_key = CMD_ERR_INVALID_ARGUMENT;
    return STATUS_CHECK_CONDITION;
}

acsi_status_t smonson_vendor_specific_command_10(logical_drive_t *device, uint8_t cmd_offset) {
    
    uint8_t subcommand = cmdbuf[cmd_offset + 1];

    // Set real-time clock
    if (subcommand == 0x80) {
        debug("Set clock");

        // Read 16 byte payload
        uint8_t datetime[16];

        set_data_mode();

        // TODO: timeout

        for (uint8_t byte = 0; byte < 16; byte++) {
            // Get next byte from FPGA
            datetime[byte] = read_byte();
        }

        debug("Received data:");
        for (uint8_t i = 0; i < 16; i++) {
            debug_hex(datetime[i], 2);
            debug_nocr(" ");
        }
        debug("");

        // Endianness is different between the AVR128DA and 68000.
        // Only year is affected, all others are bytes
        uint8_t temp = datetime[0];
        datetime[0] = datetime[1];
        datetime[1] = temp;

        rtc_set((datetime_t *)datetime);

        return STATUS_OK;
    }
    // Set real-time clock
    else if (subcommand == 0x81) {
        debug("Get clock");

        // Write 16 byte payload
        uint8_t datetime[16];
        memset(datetime, 0, 16);

        rtc_get((datetime_t *)datetime);

        // Endianness is different between the AVR128DA and 68000.
        // Only year is affected, all others are bytes
        uint8_t temp = datetime[0];
        datetime[0] = datetime[1];
        datetime[1] = temp;

        set_data_mode();

        // TODO: timeout

        for (uint8_t byte = 0; byte < 16; byte++) {
            // Send 16 bytes to FPGA
            write_byte(datetime[byte]);
        }

        debug("Sent clock data:");
        for (uint8_t i = 0; i < 16; i++) {
            debug_hex(datetime[i], 2);
            debug_nocr(" ");
        }
        debug("");

        return STATUS_OK;
    }
    
    device->sense_key = CMD_ERR_INVALID_ARGUMENT;
    return STATUS_CHECK_CONDITION;
}

// This doesn't work properly yet. I don't even know if there's much point in it existing at all.
acsi_status_t report_luns_12(logical_drive_t *device, uint8_t cmd_offset) {
    uint8_t select_report = cmdbuf[cmd_offset + 2];

    // ALLOCATION LENGTH - not used because we are only going to use 16 bytes which is the minimum anyway.
    //uint32_t allocation_length = cmdbuf[cmd_offset + 6];
    //allocation_length <<= 8;
    //allocation_length |= cmdbuf[cmd_offset + 7];
    //allocation_length <<= 8;
    //allocation_length |= cmdbuf[cmd_offset + 8];
    //allocation_length <<= 8;
    //allocation_length |= cmdbuf[cmd_offset + 9];
    // CONTROL - not used
    //uint8_t control = cmdbuf[cmd_offset + 11];

    // 0-2 report the LUNs with various confusing filtering. All other values are reserved.
    if (select_report > 2) {
        device->sense_key = CMD_ERR_INVALID_ARGUMENT;
        return STATUS_CHECK_CONDITION;
    }

    // Lun list length: 32 bit integer - 8 if we want to put one disk on each ACSI controller address for max compatability.
    // This is in bytes, so 1 device equals 8 bytes.
    write_byte(0);
    write_byte(0);
    write_byte(0);
    write_byte(1);

    // Reserved - 4 bytes
    write_byte(0);
    write_byte(0);
    write_byte(0);
    write_byte(0);

    // LUN 0 
    // A single-level LUN structure using peripheral device addressing method (byte 0, bits 7-6)
    // and bus identifier 0 (byte 0, bits 5-0)
    // and LUN number 0 (byte 1)
    // and second-, third-, and fourth-level addresses set to 0
    write_byte(0);
    write_byte(0);
    write_byte(0);
    write_byte(0);
    write_byte(0);
    write_byte(0);
    write_byte(0);
    write_byte(0);

    return STATUS_OK;
}

acsi_status_t acsi_icd_escape(logical_drive_t *device) {
    uint8_t opcode = cmdbuf[1];
    debug_nocr("  ICD extended command: ");
    debug_hex(opcode, 2);
    debug("");

    switch (opcode) {

        case INQUIRY_6:
            return scsi_inquiry(device, 1);

        case TEST_UNIT_READY_6:
            return acsi_test_unit_ready(device, 1);

        case READ_CAPACITY_10:
            return read_capacity_10(device, 1);

        case READ_10:
            return read_10(device, 1);

        case MODE_SENSE_10:
            return mode_sense_10(device, 1);

        case ULTRASATAN_VENDOR_SPECIFIC_COMMAND_10:
            return ultrasatan_command(device, 1);
        
        case SMONSON_DISK_VENDOR_SPECIFIC_COMMAND_10:
            return smonson_vendor_specific_command_10(device, 1);

        //case REPORT_LUNS_12:
        //    return report_luns_12(device, 1);
    }

    device->sense_key = CMD_ERR_INVALID_COMMAND;
    return STATUS_CHECK_CONDITION;
}