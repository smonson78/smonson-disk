#include <stdint.h>
#include "stm32f1xx.h"

#include "spi.h"

// Setup SPI hardware in fastest configuration
void spi_fast() {

    // Disable SPI
    SPI1->CR1 |= SPI_CR1_SPE;

    // Switch to Master
    SPI1->CR1 |= SPI_CR1_MSTR;

    // Baud rate control: PCLK / 2 (12MHz) - BR = 0b000
    SPI1->CR1 &= ~SPI_CR1_BR;
    //SPI1->CR1 |= SPI_CR1_BR_1; // clock /8

    // Software NSS control - with SS high
    SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;

    // Enable SPI
    SPI1->CR1 |= SPI_CR1_SPE;
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

    // MOSI - A7
    GPIOA->CRL &= ~GPIO_CRL_CNF7;
    GPIOA->CRL &= ~GPIO_CRL_MODE7;
    // Put pin in alternate function push-pull mode
    GPIOA->CRL |= GPIO_CRL_CNF7_1;
    // Set the output mode to max. 50MHz
    GPIOA->CRL |= GPIO_CRL_MODE7_1 | GPIO_CRL_MODE7_0;

    // MISO - A6
    GPIOA->CRL &= ~GPIO_CRL_CNF6;
    GPIOA->CRL &= ~GPIO_CRL_MODE6;
    // Put pin in input mode with pullup/down
    GPIOA->CRL |= GPIO_CRL_CNF6_1;
    // Set input mode
    // enable pull up resistor
    GPIOA->ODR |= GPIO_ODR_ODR6;

    // SCK - A5
    GPIOA->CRL &= ~GPIO_CRL_CNF5;
    GPIOA->CRL &= ~GPIO_CRL_MODE5;
    // Put pin in alternate function push-pull mode
    GPIOA->CRL |= GPIO_CRL_CNF5_1;
    // Set the output mode to max. 50MHz
    GPIOA->CRL |= GPIO_CRL_MODE5_1 | GPIO_CRL_MODE5_0;

    spi_fast();
}

uint8_t spi_transfer(uint8_t send)
{
	spi_start_with_value(send);
    return spi_in_nowait();
}
