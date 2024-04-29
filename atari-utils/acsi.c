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
            printf("ACSI TIMEOUT ERROR!\n");
            return 1;
        }
    }
    return 0;
}

// ACSI command, must run as superuser.
// "dmalen" appears to be in units of sectors. Requesting one sector for a small number of bytes doesn't appear
// to do any harm.
uint16_t acsi_command(uint8_t *cmd, uint8_t cmdlen, uint8_t *dmabuf, uint8_t dmalen, uint8_t direction) {

    // Set FLOCK
    *FLOCK = 0xff;

    // uint16_t dma_dir_bit = direction ? 0x100 : 0;

    // Change DMAOUT bit to reset DMA controller, and write bit 0x90 to select the sector count register
    if (direction) {
        *DMA_MODE = 0x90;
        *DMA_MODE = 0x90;
        *DMA_MODE = 0x190;

    } else {
        *DMA_MODE = 0x190;
        *DMA_MODE = 0x190;
        *DMA_MODE = 0x90;
    }

    // Load address for DMA
    *DMA_BASE_LOW = ((uint32_t)dmabuf) & 0xff;
    *DMA_BASE_MID = ((uint32_t)dmabuf >> 8) & 0xff;
    *DMA_BASE_HIGH = ((uint32_t)dmabuf >> 16) & 0xff;

    // Load sector count
    *DMA_SECTOR_COUNT = dmalen;

    // Select command register (bit 4 = 0)
    // Assert /A1 (bit 1 = 0)
    // Select /CS = external bus (bit 3 = 1)
    // Keep DMAOUT bit as-is (bit 8)
    if (direction) {
        *DMA_MODE = 0x188; // assert A1 line
    } else {
        *DMA_MODE = 0x88; // assert A1 line
    }

    // "0x00co008a: first byte of command, and mode control reg set up for next byte.
    // c = controller number left-shifted one bit e.g. C is really controller 6
    // o = 5-bit command opcode not left-shifted"
    // Send all but final byte
    for (uint8_t i = 0; i < cmdlen - 1; i++) {
        // x8a: 0b0000000D10001010
        //               ^^   ^ ^
        //               ||   | +--- /A1 line (deasserted)
        //               ||   +----- CS connected to ACSI bus
        //               |+--------- DRQ connected to 1=floppy drive 0=ACSI - but this doesn't seem to reflect reality
        //               |           but probably doesn't matter here
        //               +---------- 1 for write or 0 for read
        if (direction) {
            *DMA_DATA = (cmd[i] << 16) | 0x18a;
        } else {
            *DMA_DATA = (cmd[i] << 16) | 0x8a;
        }

        if (wait_ack()) {
            return 0x100;
        }
    }

    // Send last byte and set up for data transfer with DRQ connected to ACSI bus
    

    if (direction) {
        *DMA_DATA = (cmd[cmdlen - 1] << 16) | 0x100;
    } else {
        *DMA_DATA = (cmd[cmdlen - 1] << 16);
    }    
    if (wait_ack()) {
        return 0x100;
    }

    // Receive status byte
    *DMA_MODE = 0x8a; // select status byte
    uint16_t status = (*DMA_DATA16) & 0xff;

    // Spin for 5ms for "some peripherals"
    uint32_t time = *HZ200 + 2;
    while (*HZ200 <= time) {
    }    

    // Clear FLOCK so TOS can resume
    *FLOCK = 0;

#if 0
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
#endif
    return status;
}
