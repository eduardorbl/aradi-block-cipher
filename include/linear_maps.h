#ifndef LINEAR_MAPS_H
#define LINEAR_MAPS_H

#include <stdint.h>

// Applies the original linear map (lm) as defined in ARADI.
uint32_t lm(const uint8_t a[4], const uint8_t b[4], const uint8_t c[4], uint8_t j, uint32_t z);

// Applies the optimized linear map (lm_new) as defined in ARADI.
uint32_t lm_new(const uint8_t a[4], const uint8_t b[4], const uint8_t d[4], uint8_t j, uint32_t z);

// Applies the bit-level shuffled linear map (lm_s_nsa) as defined in the ARADI shuffle variant.
uint32_t lm_s_nsa(const uint8_t a[4], const uint8_t b[4], const uint8_t c[4], uint8_t j, uint32_t z);

// Applies the optimized shuffled linear map (lm_s_new), avoiding explicit split of interleaved bits.
uint32_t lm_s_new(const uint8_t a[4], const uint8_t b[4], const uint8_t c[4], uint8_t j, uint32_t z);

// Applies the interleaved linear map for two blocks in parallel, modifying U and L in-place.
void l_s_new(const uint8_t a[4], const uint8_t b[4], const uint8_t c[4], uint8_t j, uint32_t *U, uint32_t *L);

#endif // LINEAR_MAPS_H
