#include "stm32f1xx.h"

#include "serial.h"
#include "int.h"

// FIFOs on both send and receive
volatile uint8_t rx_buffer[RXBUFFER];
volatile uint8_t tx_buffer[TXBUFFER];
volatile uint8_t rx_ptr, rx_len, tx_ptr, tx_len, tx_idle;

void serial_cli() {
    NVIC->ICER[1] = 1 << 5;
}

void serial_sei() {
    NVIC->ISER[1] = 1 << 5;
}

void serial_init()
{
    serial_cli();

    // Empty buffers
    rx_ptr = 0;
    rx_len = 0;
    tx_ptr = 0;
    tx_len = 0;
    tx_idle = 1;

    // Enable UART1 device, enable transmitter, enable receiver
    USART1->CR1 |= USART_CR1_UE;

    // Setup baud rate
    // NOTE the datasheet is full of weird maths involving a fixed-point fractional number.
    // This can all be ignored, it just works out to a plain int in the end.
    USART1->BRR = F_PCLK2 / F_BAUD;

    // Word length is 8 bits by default

    // Parity is off by default

    // 1 stop bits by default

    // RTS/CTS disabled by default

    // Enable the transmitter (to send a dummy transfer) and receiver
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;  

    // Setup output pin for TX

    // USART1 mapped to Alternate Function pins (TX=PB6, RX=PB7)
    AFIO->MAPR |= AFIO_MAPR_USART1_REMAP;

    // Select alternate position for USART1
#if defined(USART1_ALT_POSITION)
    // Set TX pin to output (alterate function position)
    // Port B, pin 6 CNF: alterate function output, push-pull
    GPIOB->CRL &= ~GPIO_CRL_CNF6;
    GPIOB->CRL |= GPIO_CRL_CNF6_1;
    // MODE: Output, 10MHz
    GPIOB->CRL &= ~GPIO_CRL_MODE6;
    GPIOB->CRL |= GPIO_CRL_MODE6_0;	
#endif

	// Enable receive interrupt (transmit will be enabled upon sending data)
    USART1->CR1 |= USART_CR1_RXNEIE;

    // Reset USART1
    // This doesn't work
    //RCC->APB2RSTR |= RCC_APB2RSTR_USART1RST;

    // Turn on the USART1 interrupt in the system as well. This is so daft.

    serial_sei();
}

// Must be called with serial interrupts disabled
void tx_char(int8_t data)
{
    // If there's no character already being sent
    if (tx_idle)
    {
        // Mark as not idle
        tx_idle = 0;
        
	    // Put data into buffer, sends the data
        USART1->DR = data;

        // Re-enable Transmit Data Register Empty interrupt
        USART1->CR1 |= USART_CR1_TXEIE;

        return;
    }
                
    // Put data into buffer
    tx_buffer[(tx_ptr + tx_len++) % TXBUFFER] = data;
}

void serial_sendchar(int8_t data)
{
    // Wait for buffer to have room.
    while (1) {
        serial_cli();
        if (tx_len < TXBUFFER) {
            tx_char(data);
            serial_sei();
            return;
        }
        serial_sei();
        // TODO:
        //_delay_ms(1);
    }
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
    // This is more of a Harvard thing. Just call the other one.
    serial_send((char *)s);
}

// Return character from RX buffer if available, otherwise return -1
int16_t serial_receive_nowait()
{
    int16_t data;
    serial_cli();
	if (rx_len == 0) {
	    data = -1;
    } else {	    
	    data = rx_buffer[rx_ptr++];
	    rx_ptr %= RXBUFFER;
	    rx_len--;
    }
	serial_sei();
	return data;
}

int8_t serial_receive()
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

__attribute__((interrupt ("isr"))) 
void usart1_vector()
{
    // Receive buffer not empty
    if (USART1->SR & USART_SR_RXNE) {
        // Receive the data (clears the RXNE flag)
        uint8_t data = USART1->DR;
    
        // If the next byte will fit into the buffer, dump it in
        if (rx_len != RXBUFFER) {
            rx_buffer[(rx_ptr + rx_len++) % RXBUFFER] = data;
        }

        // If it didn't fit, it's lost.
    }

    // Transmitter buffer empty
    if (USART1->SR & USART_SR_TXE) {
        // End of data?
        if (tx_len == 0) {
            tx_idle = 1;
            // Must disable the TX interrupt here or else it'll be
            // an infinite loop.
            USART1->CR1 &= ~USART_CR1_TXEIE;
            // The TXE flag remains set, so this interrupt will be cleared next time a byte is sent,
            // and at the same time interrupts will be re-enabled.
        } else {
            // Load up the next character from the buffer
            USART1->DR = tx_buffer[tx_ptr++];

            tx_ptr %= TXBUFFER;
            tx_len--;
        }
    }
}
