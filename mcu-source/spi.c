#include <stdint.h>
#include "stm32c0xx.h"

#include "spi.h"

// Setup SPI hardware in fastest configuration
void spi_fast() {
    // Disable SPI
    SPI->CR1 &= ~SPI_CR1_SPE;

    // Baud rate control: PCLK / 2 (24MHz) - BR = 0b000
    SPI->CR1 &= ~SPI_CR1_BR;

    // Re-enable SPI
    SPI->CR1 |= SPI_CR1_SPE;
}

// Switch to 312.5 KHz operation
void spi_slow() {
    // Disable SPI
    SPI->CR1 &= ~SPI_CR1_SPE;

    // Baud rate control: PCLK / 128 (375KHz) - BR = 0b110
    // Must be below 5MHz for MCP7951 and below 400KHz for SD card initialisation
    SPI->CR1 &= ~SPI_CR1_BR;
    //SPI->CR1 |= 0b100 << SPI_CR1_BR_Pos; // /x32
    SPI->CR1 |= 0b111 << SPI_CR1_BR_Pos;

    // Re-enable SPI
    SPI->CR1 |= SPI_CR1_SPE;
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
    // Alternate function 0, very fast, open drain with pull-up
    SPI_MISO_PORT->MODER &= MODE_MASK(SPI_MISO_BIT);
    SPI_MISO_PORT->MODER |= MODE_ALT_FUNC(SPI_MISO_BIT);
    SPI_MISO_PORT->OSPEEDR &= OSPEED_MASK(SPI_MISO_BIT);
    SPI_MISO_PORT->OSPEEDR |= OSPEED_VFAST(SPI_MISO_BIT);
    SPI_MISO_PORT->OTYPER &= OTYPE_MASK(SPI_MISO_BIT);
    SPI_MISO_PORT->OTYPER |= OTYPE_OPEN_DRAIN(SPI_MISO_BIT);
    SPI_MISO_PORT->PUPDR &= PUPD_MASK(SPI_MISO_BIT);
    SPI_MISO_PORT->PUPDR |= PUPD_PULLUP(SPI_MISO_BIT);

    // SCK - PB3
    // Alternate function 0, very fast
    SPI_CLK_PORT->MODER &= MODE_MASK(SPI_CLK_BIT);
    SPI_CLK_PORT->MODER |= MODE_ALT_FUNC(SPI_CLK_BIT);
    SPI_CLK_PORT->OSPEEDR &= OSPEED_MASK(SPI_CLK_BIT);
    SPI_CLK_PORT->OSPEEDR |= OSPEED_VFAST(SPI_CLK_BIT);

    // Reset SPI
    RCC->APBRSTR2 |= RCC_APBRSTR2_SPI1RST;
    RCC->APBRSTR2 &= ~RCC_APBRSTR2_SPI1RST;

    // Disable SPI
    SPI->CR1 &= ~SPI_CR1_SPE;

    // Switch to Master
    SPI->CR1 |= SPI_CR1_MSTR;

    // Software NSS control - with SS high
    SPI->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;

    // Switch to 8-bit RX FIFO threshold
    SPI->CR2 |= SPI_CR2_FRXTH;

    // Force 8-bit frames, should be the default though
    SPI->CR2 &= ~SPI_CR2_DS;
    SPI->CR2 |= 0b0111 << SPI_CR2_DS_Pos;

    // Enable SPI not needed since spi_fast() will do it
    
    spi_slow();
}

uint8_t spi_transfer(uint8_t send)
{
	spi_start_with_value(send);
    return spi_in_nowait();
}
