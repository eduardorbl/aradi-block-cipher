#ifndef ARADI_H
#define ARADI_H

#include <stdint.h>

#define ARADI_NUM_ROUNDS 16

// Encrypts a 128-bit block using ARADI (4 x 32-bit words)
void aradi_encrypt(const uint32_t plaintext[4], const uint32_t round_keys[17][4], uint32_t output[4]);

// Decrypts a 128-bit block using ARADI (4 x 32-bit words)
void aradi_decrypt(const uint32_t ciphertext[4], const uint32_t round_keys[17][4], uint32_t output[4]);

// Encrypts a 128-bit block using ARADI with bit shuffling
void aradi_encrypt_shuffle(const uint32_t state[4], const uint32_t key[8], uint32_t output[4]);

// Decrypts a 128-bit block using ARADI with bit shuffling
void aradi_decrypt_shuffle(const uint32_t ciphertext[4], const uint32_t key[8], uint32_t output[4]);

#endif // ARADI_H
