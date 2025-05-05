#include "utils.h"

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
