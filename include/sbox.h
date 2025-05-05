#ifndef SBOX_H
#define SBOX_H

#include <stdint.h>

// Applies the ARADI S-box to 4 input words in place.
void sbox_layer(uint32_t *w, uint32_t *x, uint32_t *y, uint32_t *z);

// Applies the inverse ARADI S-box to 4 input words in place.
void sbox_inverse(uint32_t *w, uint32_t *x, uint32_t *y, uint32_t *z);

#endif // SBOX_H
