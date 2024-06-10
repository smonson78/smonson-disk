#include "stm32c0xx.h"

#include "stdutil.h"
#include "fpga_comm.h"
#include "serial.h"
#include "int.h"
#include "timer0.h"

// FIFOs on both send and receive
volatile uint8_t rx_buffer[RXBUFFER];
volatile uint8_t tx_buffer[TXBUFFER];
volatile uint8_t rx_ptr, rx_len, tx_ptr, tx_len, tx_idle;

void serial_cli() {
    NVIC_DisableIRQ(USART1_IRQn);
}

void serial_sei() {
    NVIC_EnableIRQ(USART1_IRQn);
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
    USART1->BRR = F_PCLK / F_BAUD;

    // Word length is 8 bits by default

    // Parity is off by default

    // 1 stop bits by default

    // RTS/CTS disabled by default

    // Enable the transmitter (to send a dummy transfer) and receiver
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;  

    // Setup output pin for TX

    // USART1 connected to PB6 (TX), PB7 (RX) alternate function 0.

    // Note: hard-coded register number due to there being 8 per register
    // Clear these - which is the default anyway - to get Alternate Function 0
    TX_PORT->AFR[0] &= ~(0b1111 << (4 * TX_BIT));
    TX_PORT->AFR[0] &= ~(0b1111 << (4 * RX_BIT));

    // Select Alternate Function for the GPIO pin modes
    TX_PORT->MODER &= MODE_MASK(TX_BIT);
    TX_PORT->MODER |= MODE_ALT_FUNC(TX_BIT);
    RX_PORT->MODER &= MODE_MASK(RX_BIT);
    RX_PORT->MODER |= MODE_ALT_FUNC(RX_BIT);

    // Speed them up
    TX_PORT->OSPEEDR &= OSPEED_MASK(TX_BIT);
    TX_PORT->OSPEEDR |= OSPEED_VFAST(TX_BIT);
    RX_PORT->OSPEEDR &= OSPEED_MASK(RX_BIT);
    RX_PORT->OSPEEDR |= OSPEED_VFAST(RX_BIT);

	// Enable receive interrupt (transmit will be enabled upon sending data)
    USART1->CR1 |= USART_CR1_RXNEIE_RXFNEIE;

    // FIFO mode is off by default

    // Turn on the USART1 interrupt
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
        USART1->TDR = data;

        // Re-enable Transmit Data Register Empty interrupt
        USART1->CR1 |= USART_CR1_TXEIE_TXFNFIE;

        return;
    }
                
    // Put data into buffer
    tx_buffer[(tx_ptr + tx_len) % TXBUFFER] = data;
    tx_len++;
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
        _delay_ms(1);
    }
}

// Send a bunch of characters at once
void serial_send(char *s)
{
    while (*s) {
        serial_sendchar(*(s++));
    }
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

// USART1 global interrupt handler
__attribute__((interrupt ("isr"))) 
void usart1_vector()
{
    // Receive buffer not empty
    if (USART1->ISR & USART_ISR_RXNE_RXFNE) {
        // Receive the data (clears the RXNE flag)
        uint8_t data = USART1->RDR;
    
        // If the next byte will fit into the buffer, dump it in
        if (rx_len != RXBUFFER) {
            rx_buffer[(rx_ptr + rx_len++) % RXBUFFER] = data;
        }

        // If it didn't fit, it's lost.
    }

    // Transmitter buffer empty
    if (USART1->ISR & USART_ISR_TXE_TXFNF) {
        // End of data?
        if (tx_len == 0) {
            tx_idle = 1;
            // Must disable the TX interrupt here or else it'll be
            // an infinite loop.
            USART1->CR1 &= ~USART_CR1_TXEIE_TXFNFIE;

            // The TXE flag remains set, so this interrupt will be cleared next time a byte is sent,
            // and at the same time interrupts will be re-enabled.
        } else {
            // Load up the next character from the buffer
            USART1->TDR = tx_buffer[tx_ptr++];

            tx_ptr %= TXBUFFER;
            tx_len--;
        }
    }    
}
