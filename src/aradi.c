#include "aradi.h"
#include "linear_maps.h"
#include "tables.h"
# include "sbox.h"

void aradi_encrypt(const uint32_t plaintext[4], const uint32_t round_keys[17][4], uint32_t output[4]) {
    uint32_t w = plaintext[0];
    uint32_t x = plaintext[1];
    uint32_t y = plaintext[2];
    uint32_t z = plaintext[3];

    for (int i = 0; i < ARADI_NUM_ROUNDS; ++i) {
        // Add round key
        w ^= round_keys[i][0];
        x ^= round_keys[i][1];
        y ^= round_keys[i][2];
        z ^= round_keys[i][3];

        // S-box layer (placeholder)
        sbox_layer(&w, &x, &y, &z);

        // Linear map
        int j = i % 4;
        w = lm_new(ARADI_A, ARADI_B, ARADI_D, j, w);
        x = lm_new(ARADI_A, ARADI_B, ARADI_D, j, x);
        y = lm_new(ARADI_A, ARADI_B, ARADI_D, j, y);
        z = lm_new(ARADI_A, ARADI_B, ARADI_D, j, z);        
    }

    // Final key XOR
    w ^= round_keys[16][0];
    x ^= round_keys[16][1];
    y ^= round_keys[16][2];
    z ^= round_keys[16][3];

    output[0] = w;
    output[1] = x;
    output[2] = y;
    output[3] = z;
}

void aradi_decrypt(const uint32_t ciphertext[4], const uint32_t round_keys[17][4], uint32_t output[4]) {
    uint32_t w = ciphertext[0];
    uint32_t x = ciphertext[1];
    uint32_t y = ciphertext[2];
    uint32_t z = ciphertext[3];

    // Initial final key XOR
    w ^= round_keys[16][0];
    x ^= round_keys[16][1];
    y ^= round_keys[16][2];
    z ^= round_keys[16][3];

    // Reverse rounds
    for (int i = ARADI_NUM_ROUNDS - 1; i >= 0; --i) {
        int j = i % 4;

        // Linear map (inverse step logically applied first in reverse)
        w = lm_new(ARADI_A, ARADI_B, ARADI_D, j, w);
        x = lm_new(ARADI_A, ARADI_B, ARADI_D, j, x);
        y = lm_new(ARADI_A, ARADI_B, ARADI_D, j, y);
        z = lm_new(ARADI_A, ARADI_B, ARADI_D, j, z);

        // Inverse S-box
        sbox_inverse(&w, &x, &y, &z);

        // Subtract round key (same as XOR in binary field)
        w ^= round_keys[i][0];
        x ^= round_keys[i][1];
        y ^= round_keys[i][2];
        z ^= round_keys[i][3];
    }

    output[0] = w;
    output[1] = x;
    output[2] = y;
    output[3] = z;
}

void aradi_encrypt_shuffle(const uint32_t plaintext[4], const uint32_t round_keys[17][4], uint32_t output[4]) {
    uint32_t w = shuffle(plaintext[0]);
    uint32_t x = shuffle(plaintext[1]);
    uint32_t y = shuffle(plaintext[2]);
    uint32_t z = shuffle(plaintext[3]);

    for (int i = 0; i < ARADI_NUM_ROUNDS; ++i) {
        // Add round key
        w ^= round_keys[i][0];
        x ^= round_keys[i][1];
        y ^= round_keys[i][2];
        z ^= round_keys[i][3];

        // S-box layer (placeholder)
        sbox_layer(&w, &x, &y, &z);

        // Linear map
        int j = i % 4;
        w = lm_s_new(ARADI_A_S, ARADI_B_S, ARADI_C_S, j, w);
        x = lm_s_new(ARADI_A_S, ARADI_B_S, ARADI_C_S, j, x);
        y = lm_s_new(ARADI_A_S, ARADI_B_S, ARADI_C_S, j, y);
        z = lm_s_new(ARADI_A_S, ARADI_B_S, ARADI_C_S, j, z);        
    }

    // Final key XOR
    w ^= round_keys[16][0];
    x ^= round_keys[16][1];
    y ^= round_keys[16][2];
    z ^= round_keys[16][3];

    w = unshuffle(w);
    x = unshuffle(x);
    y = unshuffle(y);
    z = unshuffle(z);

    output[0] = w;
    output[1] = x;
    output[2] = y;
    output[3] = z;
}

void aradi_decrypt_shuffle(const uint32_t ciphertext[4], const uint32_t round_keys[17][4], uint32_t output[4]) {
    uint32_t w = shuffle(ciphertext[0]);
    uint32_t x = shuffle(ciphertext[1]);
    uint32_t y = shuffle(ciphertext[2]);
    uint32_t z = shuffle(ciphertext[3]);

    // Initial final key XOR
    w ^= round_keys[16][0];
    x ^= round_keys[16][1];
    y ^= round_keys[16][2];
    z ^= round_keys[16][3];

    // Reverse rounds
    for (int i = ARADI_NUM_ROUNDS - 1; i >= 0; --i) {
        int j = i % 4;

        // Linear map (inverse step logically applied first in reverse)
        w = lm_s_new(ARADI_A_S, ARADI_B_S, ARADI_C_S, j, w);
        x = lm_s_new(ARADI_A_S, ARADI_B_S, ARADI_C_S, j, x);
        y = lm_s_new(ARADI_A_S, ARADI_B_S, ARADI_C_S, j, y);
        z = lm_s_new(ARADI_A_S, ARADI_B_S, ARADI_C_S, j, z);

        // Inverse S-box
        sbox_inverse(&w, &x, &y, &z);

        // Subtract round key (same as XOR in binary field)
        w ^= round_keys[i][0];
        x ^= round_keys[i][1];
        y ^= round_keys[i][2];
        z ^= round_keys[i][3];
    }

    w = unshuffle(w);
    x = unshuffle(x);
    y = unshuffle(y);
    z = unshuffle(z);

    output[0] = w;
    output[1] = x;
    output[2] = y;
    output[3] = z;
}

