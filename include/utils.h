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

// Struct used to return a pair of 32-bit integers.
typedef struct {
    uint32_t x;
    uint32_t y;
} u32pair_t;

// Performs a perfect shuffle of the bits in a 32-bit integer.
uint32_t shuffle(uint32_t z);

// Reverses the effect of the perfect shuffle on a 32-bit integer.
uint32_t unshuffle(uint32_t z);

// Swaps each adjacent pair of bits in a 32-bit integer.
uint32_t swap_two_bits(uint32_t x);

// Mixes two 32-bit words using bitwise shuffling and left rotations x by 1 and y by 3.
u32pair_t m13(uint32_t x, uint32_t y);

// Mixes two 32-bit words using bitwise shuffling, left rotation x by 9, and left rotation y by 28.
u32pair_t m928(uint32_t x, uint32_t y);

#endif // UTILS_H
