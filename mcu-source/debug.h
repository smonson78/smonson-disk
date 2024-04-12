#ifndef __DEBUG_H
#define __DEBUG_H

#include <avr/pgmspace.h>

#define DEBUG_ENABLED

extern uint8_t debug_level;

#if defined(DEBUG_ENABLED)
#define debug(x) do{ debug_fn(PSTR(x)); }while(0)
#define debug_nocr(x) do{ debug_nocr_fn(PSTR(x)); }while(0)
#define debug_decimal(x) do{ debug_decimal_fn(x); }while(0)
#define debug_hex(x, y) do{ debug_hex_fn(x, y); }while(0)
#else
#define debug(x) do{}while(0)
#define debug_nocr(x) do{}while(0)
#define debug_decimal(x) do{}while(0)
#define debug_hex(x, y) do{}while(0)
#endif

void debug_fn(const char *string);
void debug_nocr_fn(const char *string);

void debug_ram_fn(char *string);
void debug_ram_nocr(char *string);
void debug_decimal_fn(uint32_t number);
void debug_hex_fn(uint32_t number, uint8_t len);

#endif