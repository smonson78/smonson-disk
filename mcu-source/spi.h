#ifndef __SPI_H
#define __SPI_H

#include <stdint.h>
#include "stdutil.h"
#include "debug.h"

#define SPI_PIN_MOSI _BV(4)
#define SPI_PIN_MISO _BV(5)
#define SPI_PIN_CLK  _BV(6)

void spi_setup();
uint8_t spi_transfer(uint8_t send);
void spi_fast();
void spi_slow();

// Fast inlined SPI transfers
static inline void spi_wait_ready() {
	// Wait for SPI transfer completed
	while (!(SPI1->SR & SPI_SR_RXNE)) {
	}
}

static inline void spi_start_with_value(uint8_t send)
{
	SPI1->DR = send;
	while(!(SPI1->SR & SPI_SR_TXE)) {
		// Wait for send to start
	}
}

static inline void spi_start() {
	// Start SPI transfer with dummy value (e.g. for reading)
	spi_start_with_value(0xff);
}

static inline void spi_out_nowait(uint8_t send)
{
	spi_wait_ready();
	SPI1->DR = send;
	while(!(SPI1->SR & SPI_SR_TXE)) {
		// Wait for send to start
	}
}

static inline uint8_t spi_in_nowait()
{
	spi_wait_ready();
	// Return what was received
	return SPI1->DR;
}

#endif