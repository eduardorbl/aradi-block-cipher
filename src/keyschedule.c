#include "keyschedule.h"
#include "utils.h"

typedef struct {
    uint32_t x;
    uint32_t y;
} pair_u32;

// Linear mixing function m(i, j, x, y) → retorna (new-x, new-y)
static pair_u32 m(uint8_t i, uint8_t j, uint32_t x, uint32_t y) {
    uint32_t s = rotl32(y, i);
    uint32_t t = rotl32(x, j);
    pair_u32 result;
    result.y = s ^ x;
    result.x = result.y ^ t;
    return result;
}

void derive_subkeys(const uint32_t k[8], uint32_t rk[17][4]) {
    uint32_t k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];
    uint32_t k4 = k[4], k5 = k[5], k6 = k[6], k7 = k[7];

    // round key 0
    rk[0][0] = k0;
    rk[0][1] = k1;
    rk[0][2] = k2;
    rk[0][3] = k3;

    // m1
    pair_u32 r = m(1, 3, k1, k0); k1 = r.x; k0 = r.y;
    r = m(9, 28, k3, k2); k3 = r.x; k2 = r.y;
    r = m(1, 3, k5, k4); k5 = r.x; k4 = r.y;
    r = m(9, 28, k7, k6); k7 = r.x; k6 = r.y;

    rk[1][0] = k4;
    rk[1][1] = k6;
    rk[1][2] = k5;
    rk[1][3] = k7;

    uint32_t t1 = k1, t2 = k5;

    // m2
    r = m(1, 3, k2, k0); k1 = r.x; k0 = r.y;
    r = m(9, 28, k3, t1); k3 = r.x; k2 = r.y;
    r = m(1, 3, k6, k4); k5 = r.x; k4 = r.y;
    r = m(9, 28, k7, t2); k7 = r.x; k6 = r.y;

    k7 ^= 1;

    // round key 2
    rk[2][0] = k0;
    rk[2][1] = k4;
    rk[2][2] = k2;
    rk[2][3] = k6;

    for (int i = 2; i < 16; i += 2) {
        // round key i+1
        t1 = k1;
        t2 = k3;
        
        r = m(1, 3, k4, k0); k1 = r.x; k0 = r.y;
        r = m(9, 28, k6, k2); k3 = r.x; k2 = r.y;
        r = m(1, 3, k5, t1); k5 = r.x; k4 = r.y;
        r = m(9, 28, k7, t2); k7 = r.x; k6 = r.y;
        
        k7 ^= i;

        rk[i + 1][0] = k4;
        rk[i + 1][1] = k6;
        rk[i + 1][2] = k5;
        rk[i + 1][3] = k7;

        // round key i+2
        t1 = k1;
        t2 = k5;

        r = m(1, 3, k2, k0); k1 = r.x; k0 = r.y;
        r = m(9, 28, k3, t1); k3 = r.x; k2 = r.y;
        r = m(1, 3, k6, k4); k5 = r.x; k4 = r.y;
        r = m(9, 28, k7, t2); k7 = r.x; k6 = r.y;

        k7 ^= i + 1;

        rk[i + 2][0] = k0;
        rk[i + 2][1] = k4;
        rk[i + 2][2] = k2;
        rk[i + 2][3] = k6;

    }
}
