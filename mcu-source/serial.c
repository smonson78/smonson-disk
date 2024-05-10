
#include "serial.h"

// FIFOs on both send and receive
volatile uint8_t rx_buffer[RXBUFFER];
volatile uint8_t tx_buffer[TXBUFFER];
volatile uint8_t rx_ptr, rx_len, tx_ptr, tx_len, tx_idle;

void serial_init()
{
    /*
    cli();

    // Empty buffers
    rx_ptr = 0;
    rx_len = 0;
    tx_ptr = 0;
    tx_len = 0;
    tx_idle = 1;

    USART.BAUD = (F_CPU / 16 / FBAUD) * 64;

    // Enable receive and transmit
	USART.CTRLB = USART_RXEN_bm | USART_TXEN_bm;

	// Set frame format: 8 data bits
	//USART.CTRLC = SERIAL_FORMAT; // N-8-1 is the default

    // Set pin as output
	SERIAL_TX_PORT.DIRSET = SERIAL_TX_PIN;

    // Select alternate position for USART1
#if defined(USART1_ALT_POSITION)
	// TX pin output
    PORTMUX.USARTROUTEA |= _BV(2);
#endif

	// Enable receive interrupt (transmit will be enabled when sending data)
    USART.CTRLA |= USART_RXCIE_bm;

    sei();
    */
}

void tx_char(unsigned char data)
{
    /*
    cli();

    // FIXME: we don't have to disable ALL interrupts...
    if (tx_idle)
    {
        // If there's no character already being sent
        tx_idle = 0;
        
        // Re-enable Data Register Empty interrupt
        USART.CTRLA |= USART_DREIE_bm;

	    // Put data into buffer, sends the data
        USART.TXDATAL = data;
	    
        sei();
        return;
    }
                
    // Put data into buffer
    tx_buffer[(tx_ptr + tx_len++) % TXBUFFER] = data;
    sei();
    */
}

void serial_sendchar(unsigned char data)
{
    /*
    // Wait for buffer to have room. OK to spin on tx_len because it's 8 bits.
    while (1) {
        cli();
        if (tx_len < TXBUFFER) {
            tx_char(data);
            sei();
            return;
        }
        sei();
        _delay_ms(1);
    }

    */
    tx_char(data);
}

// Send a bunch of characters at once
void serial_send(char *s)
{
    while (*s) {
        serial_sendchar(*(s++));
    }
}

void serial_send_progmem(const char *s)
{
    /*
    while (pgm_read_byte(s))
    {
        serial_sendchar(pgm_read_byte(s++));
    }
    */
}

// Return character from RX buffer if available, otherwise return -1
int16_t serial_receive_nowait()
{
    int16_t data;
    //cli();
	if (rx_len == 0) {
	    data = -1;
    } else {	    
	    data = rx_buffer[rx_ptr++];
	    rx_ptr %= RXBUFFER;
	    rx_len--;
	}
	//sei();
	return data;
}

unsigned char serial_receive()
{
    int16_t data = serial_receive_nowait();
    while (data < 0)
    {
        data = serial_receive_nowait();
    }
    return data;
}

// FIXME: need to set these vector names from the USART macro (for avr128da).

// Receive Complete interrupt handler
#if 0
ISR(USART1_RXC_vect)
{
    // Receive the data (clears the interrupt bit)
    uint8_t data = USART.RXDATAL;
    
    // Ignore data that won't fit into the buffer
    if (rx_len == RXBUFFER) {
        return;
    }
    // Otherwise dump it in the buffer
    rx_buffer[(rx_ptr + rx_len++) % RXBUFFER] = data;
}

// (Transmit) Data Register Empty interrupt handler
ISR(USART1_DRE_vect)
{
    // End of data?
    if (tx_len == 0) {
        tx_idle = 1;
        // Must disable the interrupt here or else it'll be
        // an infinite loop.
        USART.CTRLA &= ~USART_DREIE_bm;
        return;
    }
    
    // Load up the next character from the buffer
    USART.TXDATAL = tx_buffer[tx_ptr++];

    tx_ptr %= TXBUFFER;
    tx_len--;
}
#endif