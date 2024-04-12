#ifndef __SDCARD_H
#define __SDCARD_H

#include <string.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay_basic.h>
#include <util/delay.h>
#include <stdint.h>

// For SD card SPI
#define SPI_CS0_PORT PORTA
#define SPI_CS0_BIT _BV(2)

#define SPI_CS1_PORT PORTA
#define SPI_CS1_BIT _BV(3)

typedef enum {
	SD_CMD_0_GO_IDLE_STATE = 0,
	SD_CMD_8_SEND_IF_COND = 8,
	SD_CMD_9_SEND_CSD = 9,
	SD_CMD_12_STOP_TRANSMISSION = 12,
	SD_CMD_13_SEND_STATUS = 13,
	SD_CMD_17_BLOCK_READ = 17,
	SD_CMD_18_MULTIPLE_BLOCK_READ = 18,
	SD_CMD_23_MULTIPLE_BLOCKS_COUNT = 23,
	SD_CMD_24_WRITE_BLOCK = 24,
	SD_CMD_25_MULTIPLE_BLOCK_WRITE = 25,
	SD_CMD_55_APPLICATION_SPECIFIC_COMMAND = 55,
	SD_CMD_58_READ_OCR = 58
} sd_command_t;

typedef enum {
	SD_APP_CMD_41_SEND_OP_COND = 41,
} sd_application_specific_command_t;

// Spec-defined special values:
// SD_CMD_TRANSMISSION_BIT is like a start bit for every command
#define SD_CMD_TRANSMISSION_BIT _BV(6)

// Used by CMD8 - I think this can be set to anything, the command just sends it back as a confirmation.
#define SD_CMD_CHECK_PATTERN 0xAA

// Sent by card to indicate the start of a data block
#define SD_CMD_BLOCK_READY_TOKEN 0xFE
#define SD_CMD_BLOCK_SEND_TOKEN 0xFC
#define SD_CMD_STOP_TRAN_TOKEN 0xFD

// Sent by card to indicate block write status
#define SD_CMD_SINGLE_BLOCK_START_TOKEN 0xFE

#define SD_CMD_BLOCK_DATA_ACCEPTED 0x05

// R1 responses - by bit
typedef enum {
	SD_CMD_R1_IN_IDLE_STATE = _BV(0),
	SD_CMD_R1_ERASE_RESET = _BV(1),
	SD_CMD_R1_ILLEGAL_COMMAND = _BV(2),
	SD_CMD_R1_CRC_ERROR = _BV(3),
	SD_CMD_R1_ERASE_SEQUENCE_ERROR = _BV(4),
	SD_CMD_R1_ADDRESS_ERROR = _BV(5),
	SD_CMD_R1_PARAMETER_ERROR = _BV(6),
	// This bit is always cleared to 0 in a valid response:
	SD_CMD_R1_RESPONSE_MSB_ZERO = _BV(7)
} sd_r1_response_t;

// R2 responses - by bit
typedef enum {
	SD_CMD_R2_CARD_LOCKED = _BV(0),
	SD_CMD_R2_ERASE_SKIP = _BV(1),			// There are two versions of bit 1, see below
	SD_CMD_R2_ERROR = _BV(2),
	SD_CMD_R2_CC_ERROR = _BV(3),
	SD_CMD_R2_CARD_ECC_FAILED = _BV(4),
	SD_CMD_R2_WP_VIOLATION = _BV(5),
	SD_CMD_R2_ERASE_PARAM = _BV(6),
	SD_CMD_R2_OUT_OF_RANGE = _BV(7),		// There are two versions of bit 7, see below
} sd_r2_response_t;

// Additional names for R2 response bits
#define SD_CMD_R2_LOCK_UNLOCK_FAILED _BV(1)
#define SD_CMD_R2_CSD_OVERWRITE _BV(7)

// R1 responses - absolute values
#define SD_RESPONSE_R1_CARD_READY 0x00
#define SD_RESPONSE_R1_IN_IDLE_STATE 0x01

// R2 responses - absolute values
#define SD_RESPONSE_R2_NO_ERROR 0x0100

// R7 responses
#define SD_VOLTAGE_33 _BV(1)
#define SD_RESPONSE_WAITING 0xFF

#define SD_OCR_VOLTAGE_33_34 _BV(5)

// ACMD41 parameter
// HCS bit is set if host supports high capacity (SDHC+) cards
#define SD_ACMD41_HIGH_CAPACITY_SUPPORT 0x40000000

typedef enum {
	SD_CARD_TYPE_SDSC = 0,
	SD_CARD_TYPE_SDHC,
	SD_CARD_TYPE_SDXC,
	// We're pretty unlikely to have to deal with SDUC.
	SD_CARD_TYPE_SDUC
} sd_card_type_t;

typedef struct {
	uint8_t bus_id;              // Bus ID for this card (0=first, 1=second etc)
	sd_card_type_t type;         // SD, SDHC, SDHX, SDUC
	uint8_t initialised;         // Flag for initialision completed
	uint8_t usable;              // Flag for card OK
	uint32_t capacity;           // In sectors
	uint8_t ccs;                 // Card Capacity Status. If 0, card uses byte addresses, if 1 then card uses sector addresses
} sdcard_state_t;

extern sdcard_state_t sdcards[2];

extern uint8_t sdcard_initialised;
uint8_t card_version;
extern uint8_t sdcard_extended_capacity;
extern uint32_t sdcard_capacity;

void sdcard_setup();
void sdcard_init();

void sdcard_defaults(sdcard_state_t *sdcard);
void sd_select(uint8_t bus_id);
void sd_unselect();
void sd_command(uint8_t cmd, uint32_t arg);
uint8_t sd_response();
uint8_t sd_response_r1();
uint8_t sd_response_r1b();
uint8_t sd_response_r2();
void sd_response_r3(uint8_t *buf);
void sd_response_r7(uint8_t *buf);
uint8_t sd_reply();

uint8_t wait_spi_response(uint8_t iterations, uint8_t ms_delay, uint8_t x);
uint8_t wait_spi_response2(uint8_t ms_delay, uint8_t x);

#endif