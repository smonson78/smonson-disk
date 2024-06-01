// Include definitions for AVR macros that I've used everywhere
#ifndef __STDUTIL_H
#define __STDUTIL_H

#define _BV(x) (1 << x)

// TODO
#define _delay_ms(x) ;
#define _delay_us(x) ;

// Conveniences for setting various registers
#define MODE_POS(p) (p * 2)
#define MODE_MASK(p) (~(0b11 << MODE_POS(p)))
#define MODE_OUTPUT(p) (0b01 << MODE_POS(p))
#define MODE_ALT_FUNC(p) (0b10 << MODE_POS(p))

#define BSR_HIGH(p) (1 << p)
#define BSR_LOW(p) (1 << (16 + p))

#define OTYPE_POS(p) (p * 2)
#define OTYPE_MASK(p) (~(0b11 << OTYPE_POS(p)))

#define OSPEED_POS(p) (p * 2)
#define OSPEED_MASK(p) (~(0b11 << OSPEED_POS(p)))
#define OSPEED_FAST(p) (0b10 << OSPEED_POS(p))
#define OSPEED_VFAST(p) (0b11 << OSPEED_POS(p))


#endif