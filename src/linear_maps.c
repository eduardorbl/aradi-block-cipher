#include "linear_maps.h"
#include "utils.h"

uint32_t lm(const uint8_t a[4], const uint8_t b[4], const uint8_t c[4], uint8_t j, uint32_t z) {
    uint16_t l = z & 0xFFFF;
    uint16_t u = z >> 16;

    uint16_t u_a = rotl16(u, a[j]);
    uint16_t l_a = rotl16(l, a[j]);
    uint16_t u_b = rotl16(u, b[j]);
    uint16_t l_c = rotl16(l, c[j]);

    uint16_t new_u = u_a ^ l_c;
    uint16_t new_l = u_b ^ l_a;

    return join_u16(new_u, new_l);
}

uint32_t lm_new(const uint8_t a[4], const uint8_t b[4], const uint8_t d[4], uint8_t j, uint32_t z) {
    uint16_t l = z & 0xFFFF;
    uint16_t u = z >> 16;

    uint16_t t = rotl16(l, d[j]) ^ u;

    uint16_t new_u = u ^ rotl16(t, a[j]);
    uint16_t new_l = l ^ rotl16(t, b[j]);

    return join_u16(new_u, new_l);
}


uint32_t lm_s_nsa(const uint8_t a[4], const uint8_t b[4], const uint8_t c[4], uint8_t j, uint32_t z) {
    uint32_t u = z & 0xAAAAAAAA; // 0xAAAAAAAA = 10101010101010101010101010101010 in binary
    uint32_t l = z & 0x55555555; // 0x55555555 = 01010101010101010101010101010101 in binary

    u = rotl32(u, b[j]);
    l = rotl32(l, c[j]);
    uint32_t t = rotl32(z, a[j]);

    z ^= u;
    z ^= l;
    z ^= t;

    return z;
}

uint32_t lm_s_new (const uint8_t a[4], const uint8_t b[4], const uint8_t c[4], uint8_t j, uint32_t z) {
    uint32_t t = rotl32(z, c[j]);
   
    t ^= z;
   
    uint32_t u = rotl32(t, a[j]);
    uint32_t l = rotl32(t, b[j]);
   
    z ^= l ^ ((u ^ l) & 0xAAAAAAAA);

    return z;

}

void l_s_new(const uint8_t a[4], const uint8_t b[4], const uint8_t c[4], uint8_t j, uint32_t *U, uint32_t *L) {
    uint32_t T = rotl32(*L, c[j]);
    T ^= *U;

    *U ^= rotl32(T, a[j]);
    *L ^= rotl32(T, b[j]);
}