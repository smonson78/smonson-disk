#ifndef __ACSI_H
#define __ACSI_H

#include <stdint.h>

typedef enum {
    DRV_ERR_NO_ERROR = 0x0,
    DRV_ERR_NO_INDEX = 0x01,
    DRV_ERR_WRITE_FAULT = 0x03,
    DRV_ERR_NOT_READY = 0x04,
    DRV_ERR_NO_TRACK_ZERO = 0x06,
    CNT_ERR_ID_ECC_ERROR = 0x10,
    CNT_ERR_DATA_ERROR = 0x11,
    CNT_ERR_RECORD_NOT_FOUND = 0x14, // Sector number non-existent
    CMD_ERR_INVALID_COMMAND = 0x20,
    CMD_ERR_INVALID_ADDRESS = 0x21,
    CMD_ERR_VOLUME_OVERFLOW = 0x23,
    CMD_ERR_INVALID_ARGUMENT = 0x24,
    CMD_ERR_INVALID_DRIVE_NUMBER = 0x25,
    CMD_ERR_BYTE_ZERO_PARITY_CHECK = 0x26,
    CMD_ERR_CONTROLLER_SELF_TEST_FAILED = 0x30,
} acsi_error_t;

typedef enum {
    STATUS_OK = 0,
    STATUS_CHECK_CONDITION = 2
} acsi_status_t;

typedef enum {
    TEST_UNIT_READY_6 = 0,
    REQUEST_SENSE_6 = 0x3,
    FORMAT_UNIT_6 = 0x4,
    REASSIGN_BLOCKS_6 = 0x07,
    READ_6 = 0x8,
    WRITE_6 = 0xa,
    INQUIRY_6 = 0x12,
    MODE_SELECT_6 = 0x15,
    MODE_SENSE_6 = 0x1a,
    ICD_ESCAPE = 0x1f,
    ULTRASATAN_VENDOR_SPECIFIC_COMMAND_10 = 0x20,
    SMONSON_DISK_VENDOR_SPECIFIC_COMMAND_10 = 0x21,
    READ_CAPACITY_10 = 0x25,
    READ_10 = 0x28,
    MODE_SENSE_10 = 0x5a,
    REPORT_LUNS_12 = 0xa0,
} acsi_opcode_t;

// Buffer and count for reading command packets from the host
#define MAX_CMD_LENGTH 17
extern uint8_t cmdbuf[MAX_CMD_LENGTH];
extern uint8_t cmd_length;

#endif