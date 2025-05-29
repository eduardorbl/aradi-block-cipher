#include "aradi.h"
#include "linear_maps.h"
#include "tables.h"
#include "sbox.h"
#include "utils.h"
#include <stdio.h>
#include "keyschedule.h"

void aradi_encrypt(const uint32_t plaintext[4], const uint32_t key[8], uint32_t output[4]) {
    uint32_t round_keys[17][4];
    derive_subkeys(key, round_keys);

    uint32_t w = plaintext[0];
    uint32_t x = plaintext[1];
    uint32_t y = plaintext[2];
    uint32_t z = plaintext[3];

    for (int i = 0; i < ARADI_NUM_ROUNDS; ++i) {
        w ^= round_keys[i][0];
        x ^= round_keys[i][1];
        y ^= round_keys[i][2];
        z ^= round_keys[i][3];

        sbox_layer(&w, &x, &y, &z);

        int j = i % 4;
        w = lm_new(ARADI_A, ARADI_B, ARADI_D, j, w);
        x = lm_new(ARADI_A, ARADI_B, ARADI_D, j, x);
        y = lm_new(ARADI_A, ARADI_B, ARADI_D, j, y);
        z = lm_new(ARADI_A, ARADI_B, ARADI_D, j, z);        
    }

    w ^= round_keys[16][0];
    x ^= round_keys[16][1];
    y ^= round_keys[16][2];
    z ^= round_keys[16][3];

    output[0] = w;
    output[1] = x;
    output[2] = y;
    output[3] = z;
}

void aradi_decrypt(const uint32_t ciphertext[4], const uint32_t key[8], uint32_t output[4]) {
    uint32_t round_keys[17][4];
    derive_subkeys(key, round_keys);

    uint32_t w = ciphertext[0];
    uint32_t x = ciphertext[1];
    uint32_t y = ciphertext[2];
    uint32_t z = ciphertext[3];

    w ^= round_keys[16][0];
    x ^= round_keys[16][1];
    y ^= round_keys[16][2];
    z ^= round_keys[16][3];

    for (int i = ARADI_NUM_ROUNDS - 1; i >= 0; --i) {
        int j = i % 4;

        w = lm_new(ARADI_A, ARADI_B, ARADI_D, j, w);
        x = lm_new(ARADI_A, ARADI_B, ARADI_D, j, x);
        y = lm_new(ARADI_A, ARADI_B, ARADI_D, j, y);
        z = lm_new(ARADI_A, ARADI_B, ARADI_D, j, z);

        sbox_inverse(&w, &x, &y, &z);

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

void aradi_encrypt_shuffle(const uint32_t state[4], const uint32_t key[8], uint32_t output[4]) {
    uint32_t rk[17][4];
    subkeys_shuffle(key, rk); 

    uint32_t w = shuffle(state[0]);
    uint32_t x = shuffle(state[1]);
    uint32_t y = shuffle(state[2]);
    uint32_t z = shuffle(state[3]);

    for (int i = 0; i < 16; ++i) {
        w ^= rk[i][0];
        x ^= rk[i][1];
        y ^= rk[i][2];
        z ^= rk[i][3];

        sbox_layer(&w, &x, &y, &z);  // S-box bit a bit (as in the original ARADI)

        int j = i % 4;
        w = lm_s_new(ARADI_A_S, ARADI_B_S, ARADI_C_S, j, w);
        x = lm_s_new(ARADI_A_S, ARADI_B_S, ARADI_C_S, j, x);
        y = lm_s_new(ARADI_A_S, ARADI_B_S, ARADI_C_S, j, y);
        z = lm_s_new(ARADI_A_S, ARADI_B_S, ARADI_C_S, j, z);
    }

    // final XOR
    w ^= rk[16][0];
    x ^= rk[16][1];
    y ^= rk[16][2];
    z ^= rk[16][3];

    // Unshuffle 
    w = unshuffle(w);
    x = unshuffle(x);
    y = unshuffle(y);
    z = unshuffle(z);

    output[0] = w;
    output[1] = x;
    output[2] = y;
    output[3] = z;
}

void aradi_decrypt_shuffle(const uint32_t ciphertext[4], const uint32_t key[8], uint32_t output[4]) {
    uint32_t rk[17][4];
    subkeys_shuffle(key, rk);  // generation of subkeys with shuffle

    uint32_t w = shuffle(ciphertext[0]);
    uint32_t x = shuffle(ciphertext[1]);
    uint32_t y = shuffle(ciphertext[2]);
    uint32_t z = shuffle(ciphertext[3]);

    w ^= rk[16][0];
    x ^= rk[16][1];
    y ^= rk[16][2];
    z ^= rk[16][3];

    for (int i = 15; i >= 0; --i) {
        int j = i % 4;

        w = lm_s_new(ARADI_A_S, ARADI_B_S, ARADI_C_S, j, w);
        x = lm_s_new(ARADI_A_S, ARADI_B_S, ARADI_C_S, j, x);
        y = lm_s_new(ARADI_A_S, ARADI_B_S, ARADI_C_S, j, y);
        z = lm_s_new(ARADI_A_S, ARADI_B_S, ARADI_C_S, j, z);

        sbox_inverse(&w, &x, &y, &z);

        w ^= rk[i][0];
        x ^= rk[i][1];
        y ^= rk[i][2];
        z ^= rk[i][3];
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

void aradi_encrypt_shuffle_2blocks(const uint32_t state[4], const uint32_t key[8], uint32_t output[4]) {
    uint32_t rk[17][4];
    subkeys_shuffle(key, rk); 

    // w = shuffle(u0 || u1); x = shuffle(l0 || l1);
    // y = shuffle(u2 || u3); z = shuffle(l2 || l3);
    uint32_t w = shuffle((state[0] & 0xFFFF0000) | ((state[1] >> 16) & 0xFFFF));
    uint32_t x = shuffle(((state[0] << 16) & 0xFFFF0000) | (state[1] & 0xFFFF));
    uint32_t y = shuffle((state[2] & 0xFFFF0000) | ((state[3] >> 16) & 0xFFFF));
    uint32_t z = shuffle(((state[2] << 16) & 0xFFFF0000) | (state[3] & 0xFFFF));
    
    for (int i = 0; i < 16; ++i) {
        w ^= rk[i][0];
        x ^= rk[i][1];
        y ^= rk[i][2];
        z ^= rk[i][3];

        sbox_layer(&w, &x, &y, &z);  // S-box

        int j = i % 4;
        l_s_new(ARADI_A_S_2, ARADI_B_S_2, ARADI_C_S_2, j, &w, &x);
        l_s_new(ARADI_A_S_2, ARADI_B_S_2, ARADI_C_S_2, j, &y, &z);
    }

    // Final XOR
    w ^= rk[16][0];
    x ^= rk[16][1];
    y ^= rk[16][2];
    z ^= rk[16][3];

    // Unshuffle
    w = unshuffle(w);
    x = unshuffle(x);
    y = unshuffle(y);
    z = unshuffle(z);

    output[0] = (w & 0xFFFF0000) | ((x >> 16) & 0xFFFF);
    output[1] = ((w << 16) & 0xFFFF0000) | (x & 0xFFFF);
    output[2] = (y & 0xFFFF0000) | ((z >> 16) & 0xFFFF);
    output[3] = ((y << 16) & 0xFFFF0000) | (z & 0xFFFF);
}

void aradi_decrypt_shuffle_2blocks(const uint32_t state[4], const uint32_t key[8], uint32_t output[4]) {
    uint32_t rk[17][4];
    subkeys_shuffle(key, rk); 

    uint32_t w = shuffle((state[0] & 0xFFFF0000) | ((state[1] >> 16) & 0xFFFF));
    uint32_t x = shuffle(((state[0] << 16) & 0xFFFF0000) | (state[1] & 0xFFFF));
    uint32_t y = shuffle((state[2] & 0xFFFF0000) | ((state[3] >> 16) & 0xFFFF));
    uint32_t z = shuffle(((state[2] << 16) & 0xFFFF0000) | (state[3] & 0xFFFF));

    w ^= rk[16][0];
    x ^= rk[16][1];
    y ^= rk[16][2];
    z ^= rk[16][3];

    for (int i = 15; i >= 0; --i) {
        int j = i % 4;

        l_s_new(ARADI_A_S_2, ARADI_B_S_2, ARADI_C_S_2, j, &w, &x);
        l_s_new(ARADI_A_S_2, ARADI_B_S_2, ARADI_C_S_2, j, &y, &z);

        sbox_layer(&w, &x, &y, &z);

        w ^= rk[i][0];
        x ^= rk[i][1];
        y ^= rk[i][2];
        z ^= rk[i][3];
    }

    w = unshuffle(w);
    x = unshuffle(x);
    y = unshuffle(y);
    z = unshuffle(z);

    output[0] = (w & 0xFFFF0000) | ((x >> 16) & 0xFFFF);
    output[1] = ((w << 16) & 0xFFFF0000) | (x & 0xFFFF);
    output[2] = (y & 0xFFFF0000) | ((z >> 16) & 0xFFFF);
    output[3] = ((y << 16) & 0xFFFF0000) | (z & 0xFFFF);
}