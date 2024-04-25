#ifndef __ACSI_H
#define __ACSI_H

#include <stdint.h>

// 16 bits: will just write the sector count
#define DMA_SECTOR_COUNT (volatile uint16_t *)(0xFF8604)
// 32 bits: ff8604 will write the data reg and ff8606 will write the mode control reg
#define DMA_DATA (volatile uint32_t *)(0xFF8604)
#define DMA_DATA16 (volatile uint16_t *)(0xFF8604)

// For read:
#define DMA_STATUS (volatile uint16_t *)(0xFF8606)
// For write:
#define DMA_MODE (volatile uint16_t *)(0xFF8606)

#define DMA_BASE_HIGH (volatile uint8_t *)(0xFF8609)
#define DMA_BASE_MID (volatile uint8_t *)(0xFF860B)
#define DMA_BASE_LOW (volatile uint8_t *)(0xFF860D)

#define GPIP (volatile uint8_t *)(0xFFFA01)

// Floppy lock
#define FLOCK (volatile uint8_t *)(0x43E)
#define HZ200 (volatile uint32_t *)(0x4BA)

#define ICD_ESCAPE_CMD 0x1F
#define SMONSON_DISK_ACSI_CONFIG_CMD 0x21

void acsi_command(uint8_t *cmd, uint16_t cmdlen, uint8_t *dmabuf, uint16_t dmalen, uint16_t direction);

#endif