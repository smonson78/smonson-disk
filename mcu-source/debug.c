
#include "debug.h"
#include "serial.h"
#include "fpga_comm.h"

uint8_t debug_level = 9;

void debug_nocr_fn(const char *string) {
    if (debug_level > 0) {
        serial_send_progmem(string);
    }
}

void debug_fn(const char *string) {
    if (debug_level > 0) {
        serial_send_progmem(string);
        serial_send_progmem("\r\n");
    }
}

void debug_ram_nocr(char *string) {
    if (debug_level > 0) {
        serial_send(string);
    }
}

void debug_ram_fn(char *string) {
    if (debug_level > 0) {
        serial_send(string);
        serial_send("\r\n");
    }
}

void debug_decimal_fn(uint32_t number) {
    uint8_t buf[16];
    uint8_t len = 0;
    if (debug_level > 0) {    
        if (number == 0) {
            serial_sendchar('0');
            return;
        }
        while (number) {
            buf[len++] = (number % 10) + '0';
            number /= 10; 
        }
        while (len) {
            serial_sendchar(buf[--len]);
        }
    }
}

void debug_hex_fn(uint32_t number, uint8_t len) {
    uint8_t digits[] = "0123456789abcdef";
    if (debug_level > 0) {
        serial_sendchar('0');
        serial_sendchar('x');
        while (len-- > 0) {
            serial_sendchar(digits[(number >> (len * 4)) & 0xf]);
        }
    }
}