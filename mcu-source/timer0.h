#ifndef __CLOCK_H
#define __CLOCK_H

#include <stdint.h>

// Where are these normally defined? I copied these from the Cube example files
#define portNVIC_SYSTICK_CTRL_REG                       ( * ( ( volatile uint32_t * ) 0xe000e010 ) )
#define portNVIC_SYSTICK_LOAD_REG                       ( * ( ( volatile uint32_t * ) 0xe000e014 ) )
#define portNVIC_SYSTICK_CURRENT_VALUE_REG      ( * ( ( volatile uint32_t * ) 0xe000e018 ) )
#define portNVIC_SYSPRI2_REG                            ( * ( ( volatile uint32_t * ) 0xe000ed20 ) )
/* ...then bits in the registers. */
#define portNVIC_SYSTICK_INT_BIT                        ( 1UL << 1UL )
#define portNVIC_SYSTICK_ENABLE_BIT                     ( 1UL << 0UL )
#define portNVIC_SYSTICK_COUNT_FLAG_BIT         ( 1UL << 16UL )
#define portNVIC_PENDSVCLEAR_BIT                        ( 1UL << 27UL )
#define portNVIC_PEND_SYSTICK_CLEAR_BIT         ( 1UL << 25UL )

// Select prescale value
#define TIMER_PRESCALER 1

// Exactly 1000Hz
#define TIMER_COUNT 48000

#define CLOCK_RATE (F_CLK / TIMER_PRESCALER / TIMER_COUNT)

extern volatile uint32_t ticks;

void init_clock();
void start_clock();
void clear_clock();
uint32_t get_clock();

// ISR
void systick_vector();

#endif
