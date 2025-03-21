#ifndef BASE32_H
#define BASE32_H

#include <stddef.h>
#include <stdint.h>

typedef uint8_t BYTE;

#define BASE32_INCORRECT_INPUT (-1)
#define BASE32_INCORRECT_OUTPUT (-2)
#define BASE32_INCORRECT_PADDING (-3)
#define BASE32_INCORRECT_SYMBOL (-4)

int base32_decode(const char *source, BYTE *destination, size_t limit);

#endif