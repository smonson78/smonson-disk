#include "stdint.h"

#include "int.h"

extern void usart1_vector(void) __attribute__ ((weak));
extern void systick_vector(void) __attribute__ ((weak));

extern void _start(void);
extern uint8_t __stack[];

// Core vectors
__attribute((section (".data.init.enter")))
void (* const __interrupt_vector[16])() = {
  (void (*const)())__stack,
  _start,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  systick_vector,
};

// STM32C031 non-core vector table to go on the end of the core one
__attribute((section (".init")))
void (* const interrupts[60])() = {
  0, // 0
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0, // 10
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0, // 20
  0,
  0,
  0,
  0,
  0,
  0,
  usart1_vector,
  0,
  0,
  0, // 30
  0,
};
