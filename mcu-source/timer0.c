#include <stdint.h>

#include "stm32f1xx.h"
#include "timer0.h"

// clock
volatile uint32_t ticks = 0;

// Overflow interrupt vector - don't need this for Arm Cortex
__attribute__((interrupt ("isr")))
void systick_vector() {
	ticks++;
}

/* Maintain a timer */
void init_clock()
{
	// Frequency of clock is F_CPU / prescaler / count
	//TIMER.SINGLE.PER = TIMER_COUNT - 1;

	portNVIC_SYSTICK_CTRL_REG |= (1 << 2); // /1 clock source

	portNVIC_SYSTICK_LOAD_REG = TIMER_COUNT - 1;

	// Get timer into "normal" mode
	// No need to change anything, it's the default

	// Select clock prescaler
	//TIMER.SINGLE.CTRLA = TIMER_PRESCALER_VALUE;

	// Enable systick interrupt
	portNVIC_SYSTICK_CTRL_REG |= portNVIC_SYSTICK_INT_BIT;
}

void start_clock()
{
	clear_clock();
	portNVIC_SYSTICK_CTRL_REG |= portNVIC_SYSTICK_ENABLE_BIT;
}

void clear_clock() {
	ticks = 0;
}

uint32_t get_clock() {
	return ticks;
}
