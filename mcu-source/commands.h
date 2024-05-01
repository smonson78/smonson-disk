#ifndef __COMMANDS_H
#define __COMMANDS_H

#include "acsi.h"
#include "sdcard.h"

typedef struct {
  sdcard_state_t *sdcard;         // Physical card where data is stored
  uint8_t acsi_id;                // ACSI device number for this unit
  uint8_t sense_key;              // returned by Request Sense
  uint8_t additional_sense_key;   //
  uint8_t media_changed;          // True if SD card reinserted
} logical_drive_t;

// The two logical drives that are possible on this device
logical_drive_t logical_drive[2];

// ACSI/SCSI commands 0 - 0x1f (6 bytes)sd_addr
// 0x0
acsi_status_t acsi_test_unit_ready(logical_drive_t* device, uint8_t cmd_offset);
// 0x3
acsi_status_t acsi_request_sense(logical_drive_t* device, uint8_t cmd_offset);
// 0x4
acsi_status_t acsi_format_unit(logical_drive_t* device, uint8_t cmd_offset);
// 0x7
acsi_status_t acsi_reassign_blocks(logical_drive_t* device, uint8_t cmd_offset);
// 0x8
acsi_status_t acsi_read(logical_drive_t* device, uint8_t cmd_offset);
// 0xa
acsi_status_t acsi_write(logical_drive_t* device, uint8_t cmd_offset);
// 0x12
acsi_status_t scsi_inquiry(logical_drive_t* device, uint8_t cmd_offset);
// 0x15
acsi_status_t acsi_mode_select(logical_drive_t* device, uint8_t cmd_offset);
// 0x1a
acsi_status_t acsi_mode_sense_6(logical_drive_t* device, uint8_t cmd_offset);

// Special ICD escape command allowing SCSI pass-through for commands in range 0x20 - 0x5f
// 0x1f
acsi_status_t acsi_icd_escape(logical_drive_t* device);

// SCSI commands 0x20 - 0x5f (10 bytes)
// 0x25
acsi_status_t read_capacity_10(logical_drive_t *device, uint8_t cmd_offset);
// 0x28
acsi_status_t read_10(logical_drive_t* device, uint8_t cmd_offset);
// 0x5a
acsi_status_t mode_sense_10(logical_drive_t *device, uint8_t cmd_offset);

// Vendor-specific (i.e. non-standard) commands
// 0x20
acsi_status_t ultrasatan_command(logical_drive_t *device, uint8_t cmd_offset);


#endif