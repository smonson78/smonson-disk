#include <stdint.h>
#include "stm32c0xx.h"

#include "spi.h"

// Setup SPI hardware in fastest configuration
void spi_fast() {

    // Disable SPI
    SPI->CR1 &= ~SPI_CR1_SPE;

    // Switch to Master
    SPI->CR1 |= SPI_CR1_MSTR;

    // Baud rate control: PCLK / 2 (24MHz) - BR = 0b000
    SPI->CR1 &= ~SPI_CR1_BR;

    // Software NSS control - with SS high
    SPI->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;

    // Enable SPI
    SPI->CR1 |= SPI_CR1_SPE;
}

// Switch to 312.5 KHz operation
void spi_slow() {
    // TODO
    // Setup SPI hardware - Master, with a /64 prescaler
    //SPI.CTRLA &= ~SPI_ENABLE_bm;
    //SPI.CTRLA = SPI_MASTER_bm | SPI_PRESC_DIV64_gc;
    //SPI.CTRLA |= SPI_ENABLE_bm;
}

void spi_setup() {
   	// SPI output pins

    // MOSI - PB5
    // Alternate function 0, very fast
    SPI_MOSI_PORT->MODER &= MODE_MASK(SPI_MOSI_BIT);
    SPI_MOSI_PORT->MODER |= MODE_ALT_FUNC(SPI_MOSI_BIT);
    SPI_MOSI_PORT->OSPEEDR &= OSPEED_MASK(SPI_MOSI_BIT);
    SPI_MOSI_PORT->OSPEEDR |= OSPEED_VFAST(SPI_MOSI_BIT);

    // MISO - PB4
    // Alternate function 0, very fast, pull-up
    SPI_MISO_PORT->MODER &= MODE_MASK(SPI_MISO_BIT);
    SPI_MISO_PORT->MODER |= MODE_ALT_FUNC(SPI_MISO_BIT);
    SPI_MISO_PORT->OSPEEDR &= OSPEED_MASK(SPI_MISO_BIT);
    SPI_MISO_PORT->OSPEEDR |= OSPEED_VFAST(SPI_MISO_BIT);
    SPI_MISO_PORT->PUPDR &= PUPD_MASK(SPI_MISO_BIT);
    SPI_MISO_PORT->PUPDR |= PUPD_PULLUP(SPI_MISO_BIT);

    // SCK - PB3
    // Alternate function 0, very fast
    SPI_CLK_PORT->MODER &= MODE_MASK(SPI_CLK_BIT);
    SPI_CLK_PORT->MODER |= MODE_ALT_FUNC(SPI_CLK_BIT);
    SPI_CLK_PORT->OSPEEDR &= OSPEED_MASK(SPI_CLK_BIT);
    SPI_CLK_PORT->OSPEEDR |= OSPEED_VFAST(SPI_CLK_BIT);
    spi_fast();
}

uint8_t spi_transfer(uint8_t send)
{
	spi_start_with_value(send);
    return spi_in_nowait();
}
