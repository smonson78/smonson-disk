#include <stdint.h>

#include "stm32c0xx.h"
#include "timer0.h"
#include "debug.h"

// clock
volatile uint32_t ticks;

// Overflow interrupt vector - don't need this for Arm Cortex
__attribute__((interrupt ("isr")))
void systick_vector() {
	ticks++;
}

/* Maintain a timer */
void init_clock()
{
	// Clock source: SYSCLK
	portNVIC_SYSTICK_CTRL_REG |= portNVIC_SYSTICK_CLK_SOURCE_BIT;

	// Frequency of clock is F_CPU / prescaler / count
	portNVIC_SYSTICK_LOAD_REG = TIMER_COUNT - 1;

	clear_clock();

	// Enable systick interrupt
	portNVIC_SYSTICK_CTRL_REG |= portNVIC_SYSTICK_INT_BIT | portNVIC_SYSTICK_ENABLE_BIT;
}

void clear_clock() {
	// TODO: clear also the NVIC counter register
	ticks = 0;
}

uint32_t get_clock() {
	return ticks;
}

// Delay X milliseconds
void _delay_ms(uint32_t time) {
    uint32_t end_time = ticks + time + 1;
		//uint32_t last = ticks;

		//debug_nocr("Starting ");
		//debug_decimal(time);
		//debug("");
    while (ticks < end_time) {
			/*if (ticks != last) {
				debug_nocr("Waiting ");
				debug_decimal(ticks);
				debug_nocr(" --> ");
				debug_decimal(end_time);
				debug("");
				last = ticks;
			}*/
    }
		
}