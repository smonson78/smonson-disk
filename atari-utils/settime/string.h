#ifndef __STRING_H
#define __STRING_H

#include <stddef.h>

void memcpy(void *dest, const void *src, size_t bytes);
void *memset(void *dest, int c, size_t bytes);
int memcmp(const void *s1, const void *s2, size_t n);

#endif
