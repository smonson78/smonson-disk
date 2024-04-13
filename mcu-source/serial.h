#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdint.h>

// Serial buffer FIFO sizes
#define RXBUFFER 2
#define TXBUFFER 255

#define FBAUD 57600L
#define SERIAL_FORMAT (DATABITS_8 | STOPBITS_1 | PARITY_NONE)

#define USART USART1
#define SERIAL_TX_PORT PORTC
#define SERIAL_TX_PIN _BV(4)
#define USART1_ALT_POSITION

// ------------- don't change anything below this line, except to add new support

#define DATABITS_8 USART_CHSIZE_8BIT_gc
#define PARITY_NONE (0)
#define STOPBITS_1 (0)

// -------------

extern volatile uint8_t rx_buffer[RXBUFFER];
extern volatile uint8_t tx_buffer[TXBUFFER];

void serial_init();
void serial_sendchar(unsigned char data);
void serial_send(char *s);
void serial_send_progmem(const char *s);
unsigned char serial_receive();
int16_t serial_receive_nowait();

#endif