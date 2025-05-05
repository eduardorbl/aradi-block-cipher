#ifndef CIPHER_H
#define CIPHER_H

#include <stdint.h>

// Applies the original linear map (lm) as defined in ARADI.
uint32_t lm(const uint8_t a[4], const uint8_t b[4], const uint8_t c[4], uint8_t j, uint32_t z);

// Applies the optimized linear map (lm_new) as defined in ARADI.
uint32_t lm_new(const uint8_t a[4], const uint8_t b[4], const uint8_t d[4], uint8_t j, uint32_t z);

#endif // CIPHER_H
