#include "sbox.h"

// ARADI S-box (bitwise logic on 32-bit words)
void sbox_layer(uint32_t *w, uint32_t *x, uint32_t *y, uint32_t *z) {
    *x ^= (*w & *y);
    *z ^= (*x & *y);
    *y ^= (*w & *z);
    *w ^= (*x & *z);
}

// ARADI S-box inverse (bitwise logic on 32-bit words)
void sbox_inverse(uint32_t *w, uint32_t *x, uint32_t *y, uint32_t *z) {
    *w ^= (*x & *z);
    *y ^= (*w & *z);
    *z ^= (*x & *y);
    *x ^= (*w & *y);
}
