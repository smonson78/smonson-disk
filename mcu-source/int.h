#ifndef __INT_H
#define __INT_H

extern void (* const interrupts[60])();

// It's dumb that I have to do all this shit manually. It's barely documented.
//volatile uint32_t *NVIC = (uint32_t *)0xE000E100;
//volatile uint32_t *NVIC_ISER0 = (uint32_t *)0xE000E100;
//volatile uint32_t *NVIC_ISER1 = (uint32_t *)0xE000E104;


#endif