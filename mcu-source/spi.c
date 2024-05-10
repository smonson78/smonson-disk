#include <stdint.h>

#include "spi.h"

// Switch to 10MHz operation
void spi_fast() {
    // Setup SPI hardware - Master, and double clock rate, with a /4 prescaler
    //SPI.CTRLA &= ~SPI_ENABLE_bm;
    //SPI.CTRLA = SPI_MASTER_bm | SPI_CLK2X_bm;
    //SPI.CTRLA |= SPI_ENABLE_bm;
}

// Switch to 312.5 KHz operation
void spi_slow() {
    // Setup SPI hardware - Master, with a /64 prescaler
    //SPI.CTRLA &= ~SPI_ENABLE_bm;
    //SPI.CTRLA = SPI_MASTER_bm | SPI_PRESC_DIV64_gc;
    //SPI.CTRLA |= SPI_ENABLE_bm;
}

void spi_setup() {
   	// SPI output pins
    //SPI_PORT.DIRSET = SPI_PIN_MOSI | SPI_PIN_CLK;

    // enable pull up resistor in MISO
    //SPI_PORT.PINCONFIG = PORT_PULLUPEN_bm;
    //SPI_PORT.PINCTRLUPD = SPI_PIN_MISO;

    // Disable Slave Select line which, as master, we don't want
    //SPI.CTRLB = SPI_SSD_bm;

    spi_fast();
}

uint8_t spi_transfer(uint8_t send)
{
	spi_start_with_value(send);
    return spi_in_nowait();
}
