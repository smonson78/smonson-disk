#ifndef __CLOCK_H
#define __CLOCK_H

#include <stdint.h>

// Which of the 7 clock units we will use. There are 2x "A", 4x "B", 1x "D" type. Type A supports /64 prescaler.
#define TIMER TCA0
#define OVF_VECTOR_NAME TCA0_OVF_vect

// Select prescale value and OCR0A (total count) value
#define TIMER_PRESCALER 64

// Exactly 100Hz
#define TIMER_COUNT 3125

// Add other values here if needed
#if TIMER_PRESCALER==64
#define TIMER_PRESCALER_VALUE TCA_SINGLE_CLKSEL_DIV64_gc
#endif

#define CLOCK_RATE (F_CLK / TIMER_PRESCALER / TIMER_COUNT)

extern volatile uint32_t ticks;

void init_clock();
void start_clock();
void clear_clock();
uint32_t get_clock();

#endif
