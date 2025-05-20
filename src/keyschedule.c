#include "keyschedule.h"
#include "utils.h"
#include <stdio.h>

void subkeys_shuffle(const uint32_t key[8], uint32_t rk[17][4]) {
    uint32_t k[8];

    for (int i = 0; i < 8; i++) {
        k[i] = shuffle(key[i]);
    }

    uint32_t k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];
    uint32_t k4 = k[4], k5 = k[5], k6 = k[6], k7 = k[7];

    // LT[i] = shuffle(i)
    uint32_t LT[16] = {0, 1, 4, 5, 16, 17, 20, 21, 64, 65, 68, 69, 80, 81, 84, 85};
    rk[0][0] = k0;
    rk[0][1] = k1;
    rk[0][2] = k2;
    rk[0][3] = k3;
    u32pair_t r;

    r = m13(k1, k0); k1 = r.x; k0 = r.y;
    r = m928(k3, k2); k3 = r.x; k2 = r.y;

    r = m13(k5, k4); k5 = r.x; k4 = r.y;
    r = m928(k7, k6); k7 = r.x; k6 = r.y;

    rk[1][0] = k4;
    rk[1][1] = k6;
    rk[1][2] = k5;
    rk[1][3] = k7;

    uint32_t t1 = k1, t3 = k5;

    r = m13(k2, k0); k1 = r.x; k0 = r.y;
    r = m928(k3, t1); k3 = r.x; k2 = r.y;

    r = m13(k6, k4); k5 = r.x; k4 = r.y;
    r = m928(k7, t3); k7 = r.x; k6 = r.y;

    k7 ^= 1;

    rk[2][0] = k0;
    rk[2][1] = k4;
    rk[2][2] = k2;
    rk[2][3] = k6;

    for (int i = 2; i < 16; i += 2) {
        t1 = k1;
        t3 = k3;

        r = m13(k4, k0); k1 = r.x; k0 = r.y;
        r = m928(k6, k2); k3 = r.x; k2 = r.y;

        r = m13(k5, t1); k5 = r.x; k4 = r.y;
        r = m928(k7, t3); k7 = r.x; k6 = r.y;

        k7 ^= LT[i];

        rk[i + 1][0] = k4;
        rk[i + 1][1] = k6;
        rk[i + 1][2] = k5;
        rk[i + 1][3] = k7;

        t1 = k1;
        t3 = k5;

        r = m13(k2, k0); k1 = r.x; k0 = r.y;
        r = m928(k3, t1); k3 = r.x; k2 = r.y;

        r = m13(k6, k4); k5 = r.x; k4 = r.y;
        r = m928(k7, t3); k7 = r.x; k6 = r.y;

        k7 ^= LT[i + 1];

        rk[i + 2][0] = k0;
        rk[i + 2][1] = k4;
        rk[i + 2][2] = k2;
        rk[i + 2][3] = k6;
    }

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
    u32pair_t r = m13(k1, k0); k1 = r.x; k0 = r.y;
    r = m928(k3, k2); k3 = r.x; k2 = r.y;
    r = m13(k5, k4); k5 = r.x; k4 = r.y;
    r = m928(k7, k6); k7 = r.x; k6 = r.y;

    rk[1][0] = k4;
    rk[1][1] = k6;
    rk[1][2] = k5;
    rk[1][3] = k7;

    uint32_t t1 = k1, t2 = k5;

    // m2
    r = m13(k2, k0); k1 = r.x; k0 = r.y;
    r = m928(k3, t1); k3 = r.x; k2 = r.y;
    r = m13(k6, k4); k5 = r.x; k4 = r.y;
    r = m928(k7, t2); k7 = r.x; k6 = r.y;

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
        
        r = m13(k4, k0); k1 = r.x; k0 = r.y;
        r = m928(k6, k2); k3 = r.x; k2 = r.y;
        r = m13(k5, t1); k5 = r.x; k4 = r.y;
        r = m928(k7, t2); k7 = r.x; k6 = r.y;
        
        k7 ^= i;

        rk[i + 1][0] = k4;
        rk[i + 1][1] = k6;
        rk[i + 1][2] = k5;
        rk[i + 1][3] = k7;

        // round key i+2
        t1 = k1;
        t2 = k5;

        r = m13(k2, k0); k1 = r.x; k0 = r.y;
        r = m928(k3, t1); k3 = r.x; k2 = r.y;
        r = m13(k6, k4); k5 = r.x; k4 = r.y;
        r = m928(k7, t2); k7 = r.x; k6 = r.y;

        k7 ^= i + 1;

        rk[i + 2][0] = k0;
        rk[i + 2][1] = k4;
        rk[i + 2][2] = k2;
        rk[i + 2][3] = k6;

    }
}
