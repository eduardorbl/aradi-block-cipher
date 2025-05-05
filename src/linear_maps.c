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
