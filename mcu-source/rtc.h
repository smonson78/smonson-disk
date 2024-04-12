#ifndef __RTC_H
#define __RTC_H

#include <avr/io.h>

#define CLOCK_PORT PORTE
#define CLOCK_PIN _BV(1)

#define MCP7951_READ 0b00010011
#define MCP7951_WRITE 0b00010010

typedef struct {
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
  uint8_t day;
  uint8_t month;
  uint8_t year;
} datetime_t;

void rtc_setup();
void rtc_read(datetime_t *dest);
void rtc_write(uint8_t addr, uint8_t *data, uint8_t len);

#endif