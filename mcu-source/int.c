#include "stdint.h"

#include "int.h"

extern void usart1_vector(void) __attribute__ ((weak));

// STM32F103 non-core interrupt vectors table to go on the end of the libc one

__attribute((section (".init")))
void (* const interrupts[60])() = {
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
  0,
  0,
  0,
  0, // 30
  0,
  0,
  0,
  0,
  0,
  0,
  usart1_vector,
  0,
  0,
  0, // 40
  0,
  0,
};
