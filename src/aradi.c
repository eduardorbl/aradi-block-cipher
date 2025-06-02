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

void aradi_encrypt_shuffle_2blocks(const uint32_t state0[4], const uint32_t state1[4], const uint32_t key[8], uint32_t output0[4], uint32_t output1[4]) {
    uint32_t rk[17][4];
    subkeys_shuffle(key, rk);  // generation of subkeys with shuffle

    // Helper function to assemble pairs U and L from 2 blocks
    #define BUILD_PAIR(u0, u1)   (((u0 & 0xFFFF0000)) | ((u1 & 0xFFFF0000) >> 16))
    #define BUILD_PAIR_L(l0, l1) (((l0 & 0x0000FFFF) << 16) | (l1 & 0x0000FFFF))

    // Initial shuffle of pairs
    uint32_t U0 = shuffle(BUILD_PAIR(state0[0], state1[0]));
    uint32_t L0 = shuffle(BUILD_PAIR_L(state0[0], state1[0]));
    uint32_t U1 = shuffle(BUILD_PAIR(state0[1], state1[1]));
    uint32_t L1 = shuffle(BUILD_PAIR_L(state0[1], state1[1]));
    uint32_t U2 = shuffle(BUILD_PAIR(state0[2], state1[2]));
    uint32_t L2 = shuffle(BUILD_PAIR_L(state0[2], state1[2]));
    uint32_t U3 = shuffle(BUILD_PAIR(state0[3], state1[3]));
    uint32_t L3 = shuffle(BUILD_PAIR_L(state0[3], state1[3]));

    // 16 rounds
    for (int i = 0; i < 16; ++i) {
        U0 ^= rk[i][0]; L0 ^= rk[i][0];
        U1 ^= rk[i][1]; L1 ^= rk[i][1];
        U2 ^= rk[i][2]; L2 ^= rk[i][2];
        U3 ^= rk[i][3]; L3 ^= rk[i][3];

        sbox_layer(&U0, &U1, &U2, &U3);
        sbox_layer(&L0, &L1, &L2, &L3);

        int j = i % 4;
        l_s_new(ARADI_A_S_2, ARADI_B_S_2, ARADI_C_S_2, j, &U0, &L0);
        l_s_new(ARADI_A_S_2, ARADI_B_S_2, ARADI_C_S_2, j, &U1, &L1);
        l_s_new(ARADI_A_S_2, ARADI_B_S_2, ARADI_C_S_2, j, &U2, &L2);
        l_s_new(ARADI_A_S_2, ARADI_B_S_2, ARADI_C_S_2, j, &U3, &L3);
    }

    // Final XOR
    U0 ^= rk[16][0]; L0 ^= rk[16][0];
    U1 ^= rk[16][1]; L1 ^= rk[16][1];
    U2 ^= rk[16][2]; L2 ^= rk[16][2];
    U3 ^= rk[16][3]; L3 ^= rk[16][3];

    // Unshuffle + separation of blocks (inverse of the compression done at the beginning)
    uint32_t wu = unshuffle(U0), wl = unshuffle(L0);
    output0[0] = (wu & 0xFFFF0000) | ((wl & 0xFFFF0000) >> 16);
    output1[0] = ((wu & 0x0000FFFF) << 16) | (wl & 0x0000FFFF);

    wu = unshuffle(U1); wl = unshuffle(L1);
    output0[1] = (wu & 0xFFFF0000) | ((wl & 0xFFFF0000) >> 16);
    output1[1] = ((wu & 0x0000FFFF) << 16) | (wl & 0x0000FFFF);

    wu = unshuffle(U2); wl = unshuffle(L2);
    output0[2] = (wu & 0xFFFF0000) | ((wl & 0xFFFF0000) >> 16);
    output1[2] = ((wu & 0x0000FFFF) << 16) | (wl & 0x0000FFFF);

    wu = unshuffle(U3); wl = unshuffle(L3);
    output0[3] = (wu & 0xFFFF0000) | ((wl & 0xFFFF0000) >> 16);
    output1[3] = ((wu & 0x0000FFFF) << 16) | (wl & 0x0000FFFF);

    #undef BUILD_PAIR
    #undef BUILD_PAIR_L
}

void aradi_decrypt_shuffle_2blocks(const uint32_t ciphertext0[4], const uint32_t ciphertext1[4], const uint32_t key[8], uint32_t output0[4], uint32_t output1[4]) {
    // TODO
}