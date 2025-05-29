#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "aradi.h" 
#include "keyschedule.h"

// Helper function to print 32-bit vectors
void print_block(const char* label, const uint32_t block[4]) {
    printf("%s: ", label);
    for (int i = 0; i < 4; ++i)
        printf("%08x ", block[i]);
    printf("\n");
}

// Generate random 128-bit block (4 x uint32_t)
void random_block(uint32_t block[4]) {
    for (int i = 0; i < 4; ++i)
        block[i] = ((uint32_t)rand() << 16) | (rand() & 0xFFFF);
}

// Generate random 256-bit key (8 x uint32_t)
void random_key(uint32_t key[8]) {
    for (int i = 0; i < 8; ++i)
        key[i] = ((uint32_t)rand() << 16) | (rand() & 0xFFFF);
}

// Check if two blocks of 4 words are equal
int blocks_equal(const uint32_t a[4], const uint32_t b[4]) {
    return memcmp(a, b, 4 * sizeof(uint32_t)) == 0;
}

int main() {
    srand((unsigned int)time(NULL)); // Initialize random number generator

    const int N = 1000;
    int errors = 0;

    for (int i = 0; i < N; ++i) {
        uint32_t plaintext[4], ciphertext[4], decrypted[4];
        uint32_t key[8];

        random_block(plaintext);
        random_key(key);

        // Choose between shuffle or no shuffle:
        aradi_encrypt_shuffle(plaintext, key, ciphertext);
        aradi_decrypt_shuffle(ciphertext, key, decrypted);

        if (!blocks_equal(plaintext, decrypted)) {
            printf("❌ Error in test %d\n", i);
            print_block("Plaintext", plaintext);
            print_block("Ciphertext", ciphertext);
            print_block("Decrypted", decrypted);
            ++errors;
        }
    }

    if (errors == 0)
        printf("✅ All %d tests passed successfully!\n", N);
    else
        printf("⚠️ %d out of %d tests failed.\n", errors, N);

    return 0;
}
