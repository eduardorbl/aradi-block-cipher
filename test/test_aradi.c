#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "aradi.h"
#include "keyschedule.h"

void print_block(const char *label, const uint32_t block[4]) {
    printf("%s: ", label);
    for (int i = 0; i < 4; ++i) {
        printf("%08x ", block[i]);
    }
    printf("\n");
}

int block_equals(const uint32_t a[4], const uint32_t b[4]) {
    return memcmp(a, b, sizeof(uint32_t) * 4) == 0;
}

int main() {
    struct {
        uint32_t key[8];
        uint32_t plaintext[4];
        uint32_t expected[4];
    } tests[] = {
        {
            {0x03020100,0x07060504,0x0b0a0908,0x0f0e0d0c, 0x13121110,0x17161514,0x1b1a1918,0x1f1e1d1c},
            {0x00000000,0x00000000,0x00000000,0x00000000},
            {0x3f09abf4,0x00e3bd74,0x03260def,0xb7c53912}
        },
        {
            {0x03020100,0x07060504,0x0b0a0908,0x0f0e0d0c, 0x13121110,0x17161514,0x1b1a1918,0x1f1e1d1c},
            {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF},
            {0x2541e086,0x24cdb514,0xd9b78fc9,0xbf3e058c}
        },
        {
            {0x00000000,0x00000000,0x00000000,0x00000000, 0x00000000,0x00000000,0x00000000,0x00000000},
            {0x00000000,0x00000000,0x00000000,0x00000000},
            {0xd06c8ab7,0x5d191521,0x7a9b3d47,0x052f6870}
        },
        {
            {0x03020100,0x07060504,0x0b0a0908,0x0f0e0d0c, 0x13121110,0x17161514,0x1b1a1918,0x1f1e1d1c},
            {0x00000000,0x00000000,0x00000000,0x00000000},
            {0x3f09abf4,0x00e3bd74,0x03260def,0xb7c53912}
        }        
    };

    for (int i = 0; i < 4; ++i) {
        uint32_t rk[17][4];
        uint32_t encrypted[4];
        uint32_t decrypted[4];

        printf("Test %d:\n", i + 1);
        derive_subkeys(tests[i].key, rk);

        aradi_encrypt(tests[i].plaintext, rk, encrypted);
        print_block("Expected", tests[i].expected);
        print_block("Encrypted", encrypted);
        printf("%s\n", block_equals(encrypted, tests[i].expected) ? "✅ Encrypt passed!" : "❌ Encrypt failed!");

        aradi_decrypt(encrypted, rk, decrypted);
        print_block("Decrypted", decrypted);
        print_block("Original ", tests[i].plaintext);
        printf("%s\n\n", block_equals(decrypted, tests[i].plaintext) ? "✅ Decrypt passed!" : "❌ Decrypt failed!");
    }

    return 0;
}
