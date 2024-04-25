#include <libc.h>

#include "acsi.h"

uint16_t wait_ack() {
    // Test bit 5 of GPIP for IRQ
    // Wait for ACK - bit 5 of GPIP is cleared when target device is ready for next byte

    // Spin for 5ms
    uint32_t time = *HZ200 + 2;
    while (*HZ200 <= time) {
    }

    // New timeout - 3 seconds
    time = *HZ200 + 600;

    while ((*GPIP) & 0b00100000) {
        if (*HZ200 >= time) {
            printf("TIMEOUT ERROR!\n");
            return 1;
        }
    }
    return 0;
}

// ACSI command, must run as superuser.
// "dmalen" appears to be in units of sectors. Requesting one sector for a small number of bytes doesn't appear
// to do any harm.
void acsi_command(uint8_t *cmd, uint16_t cmdlen, uint8_t *dmabuf, uint16_t dmalen, uint16_t direction) {

    // Set FLOCK
    *FLOCK = 0xff;

    uint16_t dma_dir_bit = direction ? 0x100 : 0;

    // Change DMAOUT bit to reset DMA controller, and write bit 0x90 to select the sector count register
    *DMA_MODE = 0x190;
    *DMA_MODE = 0x90;
    *DMA_MODE = 0x90 | dma_dir_bit;

    // Load sector count
    *DMA_SECTOR_COUNT = dmalen;

    // Load address for DMA
    *DMA_BASE_LOW = ((uint32_t)dmabuf) & 0xff;
    *DMA_BASE_MID = ((uint32_t)dmabuf >> 8) & 0xff;
    *DMA_BASE_HIGH = ((uint32_t)dmabuf >> 16) & 0xff;

    // Write command byte 1
    *DMA_MODE = 0x88 | dma_dir_bit; // assert A1 line

    // "0x00co008a: first byte of command, and mode control reg set up for next byte.
    // c = controller number left-shifted one bit e.g. C is really controller 6
    // o = 5-bit command opcode not left-shifted"
    // Send all but final byte
    for (uint8_t i = 0; i < cmdlen - 1; i++) {
        // 8a: 0b10001010
        //       ^   ^ ^
        //       |   | +--- /A1 line (deasserted)
        //       |   +----- CS connected to ACSI bus
        //       +--------- DRQ connected to floppy drive
        *DMA_DATA = (cmd[i] << 16) | 0x8a | dma_dir_bit;
        if (wait_ack()) {
            return;
        }
    }

    // Send last byte (also 0) and set up for data transfer with DRQ connected to ACSI bus
    *DMA_DATA = (cmd[cmdlen - 1] << 16) | dma_dir_bit;
    if (wait_ack()) {
        return;
    }

    // Receive status byte
    *DMA_STATUS = 0x8a; // select status byte
    uint16_t status = (*DMA_DATA16) & 0xff;

    // Spin for 5ms for "some peripherals"
    uint32_t time = *HZ200 + 2;
    while (*HZ200 <= time) {
    }    

    // Clear FLOCK so TOS can resume
    *FLOCK = 0;

    printf("Payload:\n");
    for (uint8_t i = 0; i < 16; i++) {
        if (i % 16 == 0) {
            printf("0x%04x: ", i);
        }

        printf("%02x", dmabuf[i]);

        if (i % 16 == 15) {
            printf("\n");
        }
    }

    printf("Received status byte 0x%02x\n", status);
}
