#include "utils.h"
#include <stdint.h>

typedef struct {
    uint32_t x;
    uint32_t y;
} u32pair_t;

uint16_t rotl16(uint16_t x, uint8_t n) {
    n %= 16;
    return ((x << n) | (x >> (16 - n))) & 0xFFFF;
}

uint32_t join_u16(uint16_t high, uint16_t low) {
    return ((uint32_t)high << 16) | (uint32_t)low;
}

void split_u32(uint32_t value, uint16_t *high, uint16_t *low) {
    if (high) *high = (uint16_t)(value >> 16);
    if (low)  *low  = (uint16_t)(value & 0xFFFF);
}

uint32_t rotl32(uint32_t x, uint8_t n) {
    n %= 32;
    return ((x << n) | (x >> (32 - n))) & 0xFFFFFFFF;
}

uint32_t shuffle(uint32_t z) {
    uint32_t t = (z ^ (z >> 8)) & 0x0000FF00;
    z ^= t ^ (t << 8);
    
    t = (z ^ (z >> 4)) & 0x00F000F0;
    z ^= t ^ (t << 4);

    t = (z ^ (z >> 2)) & 0x0C0C0C0C;
    z ^= t ^ (t << 2);

    t = (z ^ (z >> 1)) & 0x22222222;
    z ^= t ^ (t << 1);

    return z;
}

uint32_t unshuffle(uint32_t z) {
    uint32_t t = (z ^ (z >> 1)) & 0x22222222;
    z ^= t ^ (t << 1);

    t = (z ^ (z >> 2)) & 0x0C0C0C0C;
    z ^= t ^ (t << 2);

    t = (z ^ (z >> 4)) & 0x00F000F0;
    z ^= t ^ (t << 4);

    t = (z ^ (z >> 8)) & 0x0000FF00;
    z ^= t ^ (t << 8);

    return z;
}

uint32_t swap_two_bits(uint32_t x) {
    uint32_t mask = (1 << 32) - 1;
    uint32_t t = (x << 1) & mask;
    uint32_t sx = t ^ ((t ^ (x >> 1)) & mask);
    return sx;
}

u32pair_t m13(uint32_t x, uint32_t y) {
    uint32_t sx = swap_two_bits(x);
    uint32_t sy = swap_two_bits(y);

    uint32_t s = ((y << 2) | (sy >> 30)) & 0xFFFFFFFF;  // rot_esq_2(y)
    uint32_t t = ((x << 6) | (sx >> 26)) & 0xFFFFFFFF;  // rot_esq_6(x)

    uint32_t y_out = s ^ x;
    uint32_t x_out = y_out ^ t;

    return (u32pair_t){x_out, y_out};
}

u32pair_t m928(uint32_t x, uint32_t y) {
    uint32_t sx = swap_two_bits(x);
    uint32_t sy = swap_two_bits(y);

    uint32_t s = ((y << 18) | (sy >> 14)) & 0xFFFFFFFF; // rot_esq_18(y)
    uint32_t t = ((sx << 24) | (x >> 8)) & 0xFFFFFFFF;  // rot_dir_4(x)

    uint32_t y_out = s ^ x;
    uint32_t x_out = y_out ^ t;

    return (u32pair_t){x_out, y_out};
}
