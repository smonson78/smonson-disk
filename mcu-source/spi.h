#ifndef __SPI_H
#define __SPI_H

#include <stdint.h>

#define SPI SPI0
#define SPI_PORT PORTA
#define SPI_PIN_MOSI _BV(4)
#define SPI_PIN_MISO _BV(5)
#define SPI_PIN_CLK  _BV(6)

void spi_setup();
uint8_t spi_transfer(uint8_t send);
void spi_fast();
void spi_slow();

// Fast inlined SPI transfers
static inline void spi_wait_ready() {
	// Wait for SPI hardware to be idle
	while (!(SPI.INTFLAGS & SPI_IF_bm)) {
	}
}

static inline void spi_start_with_value(uint8_t send)
{
	SPI.DATA = send;
}

static inline void spi_start() {
	// Start SPI transfer with dummy value (e.g. for reading)
	spi_start_with_value(0xff);
}

static inline void spi_out_nowait(uint8_t send)
{
	spi_wait_ready();
	SPI.DATA = send;
}

static inline uint8_t spi_in_nowait()
{
	spi_wait_ready();
	// Return what was received
	return SPI.DATA;
}

#endif