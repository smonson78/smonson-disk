#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdint.h>

#include "stm32c0xx.h"

#define TX_PORT GPIOB
#define TX_BIT 6
#define RX_PORT GPIOB
#define RX_BIT 7


// Serial buffer FIFO sizes
#define RXBUFFER 128
#define TXBUFFER 128

#define F_BAUD 57600L
#define SERIAL_FORMAT (DATABITS_8 | STOPBITS_1 | PARITY_NONE)

//#define SERIAL_TX_PORT GPIOB
//#define SERIAL_TX_PIN _BV(6)
#define USART1_ALT_POSITION

// ------------- don't change anything below this line, except to add new support

// TODO
#define DATABITS_8 0 
#define PARITY_NONE (0)
#define STOPBITS_1 (0)

// -------------

extern volatile uint8_t rx_buffer[RXBUFFER];
extern volatile uint8_t tx_buffer[TXBUFFER];

void serial_init();
void serial_sendchar(int8_t data);
void serial_send(char *s);
void serial_send_progmem(const char *s);
int8_t serial_receive();
int16_t serial_receive_nowait();

void usart1_vector();

#endif