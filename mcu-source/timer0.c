#include <stdint.h>

#include "timer0.h"

// clock
volatile uint32_t ticks = 0;

#if 0
// Overflow interrupt vector
ISR(OVF_VECTOR_NAME)
{
	ticks++;

	// Acknowledge the interrupt (required)
	TIMER.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}
#endif
/* Maintain a timer */
void init_clock()
{
	// Frequency of clock is F_CPU / prescaler / count
	//TIMER.SINGLE.PER = TIMER_COUNT - 1;

	// Get timer into "normal" mode
	// No need to change anything, it's the default

	// Select clock prescaler
	//TIMER.SINGLE.CTRLA = TIMER_PRESCALER_VALUE;
}

void start_clock()
{
	//cli();
	// Start clock from 0
	ticks = 0;

	// Enable timer
	//TIMER.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;

	// Enable overflow interrupt
	//TIMER.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
	//sei();
}

void clear_clock() {
	//cli();
	ticks = 0;
	//sei();
}

uint32_t get_clock() {
	//cli();
	uint32_t clock = ticks;
	//sei();
	return clock;
}
