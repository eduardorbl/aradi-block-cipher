#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

// Rotates a 16-bit integer x to the left by n bits.
uint16_t rotl16(uint16_t x, uint8_t n);

// Joins two 16-bit halves into a 32-bit integer: (high << 16) | low
uint32_t join_u16(uint16_t high, uint16_t low);

// Splits a 32-bit integer into two 16-bit halves.
void split_u32(uint32_t value, uint16_t *high, uint16_t *low);

// Rotates a 32-bit integer x to the left by n bits.
uint32_t rotl32(uint32_t x, uint8_t n);

#endif // UTILS_H
