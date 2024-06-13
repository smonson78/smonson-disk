// Include definitions for AVR macros that I've used everywhere
#ifndef __STDUTIL_H
#define __STDUTIL_H

#define _BV(x) (1 << x)

// Probably wrong
#define _delay_us(x) do { for (volatile uint_fast32_t _delay_us_count = 0; \
    _delay_us_count < x; _delay_us_count++) {} } while(0);

// Conveniences for setting various registers
#define MODE_POS(p) (p * 2)
#define MODE_MASK(p) (~(0b11 << MODE_POS(p)))
#define MODE_BITS(p) (0b11 << MODE_POS(p))
#define MODE_OUTPUT(p) (0b01 << MODE_POS(p))
#define MODE_ALT_FUNC(p) (0b10 << MODE_POS(p))

#define BSR_HIGH(p) (1 << p)
#define BSR_LOW(p) (1 << (16 + p))

#define OTYPE_POS(p) (p)
#define OTYPE_MASK(p) (~(0b1 << OTYPE_POS(p)))
// Push-pull is default (0)
#define OTYPE_OPEN_DRAIN(p) (0b1 << OTYPE_POS(p))

#define PUPD_POS(p) (p * 2)
#define PUPD_MASK(p) (~(0b11 << PUPD_POS(p)))
#define PUPD_PULLUP(p) (0b01 << PUPD_POS(p))
#define PUPD_PULLDOWN(p) (0b10 << PUPD_POS(p))

#define OSPEED_POS(p) (p * 2)
#define OSPEED_MASK(p) (~(0b11 << OSPEED_POS(p)))
#define OSPEED_SLOW(p) (0b01 << OSPEED_POS(p))
#define OSPEED_FAST(p) (0b10 << OSPEED_POS(p))
#define OSPEED_VFAST(p) (0b11 << OSPEED_POS(p))

#define BYTE_ACCESS(x) (*(volatile uint8_t *)(&x))
#define HALFWORD_ACCESS(x) (*(volatile uint16_t *)(&x))
#endif