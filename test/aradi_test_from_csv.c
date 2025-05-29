#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "aradi.h"
#include "keyschedule.h"
#include "utils.h"

// Converts hexadecimal string to uint32_t
uint32_t hex_to_u32(const char *hex) {
    uint32_t val;
    sscanf(hex, "%x", &val);
    return val;
}

// Checks equality between two blocks
int block_equals(const uint32_t a[4], const uint32_t b[4]) {
    return memcmp(a, b, sizeof(uint32_t) * 4) == 0;
}

// Tests encryption based on the new CSV (pt + key + ct)
void test_from_csv(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening CSV");
        return;
    }

    char line[1024];
    fgets(line, sizeof(line), fp); // skip header

    int block_id = 0, passed = 0, total = 0;

    while (fgets(line, sizeof(line), fp)) {
        char *tokens[16];
        char *ptr = strtok(line, ",\r\n");
        int i = 0;
        while (ptr && i < 16) {
            tokens[i++] = ptr;
            ptr = strtok(NULL, ",\r\n");
        }

        if (i != 16) {
            printf("❌ Invalid line with %d columns\n", i);
            continue;
        }

        uint32_t pt[4], key[8], expected[4], encrypted[4], decrypted[4];

        for (int j = 0; j < 4; j++) pt[j] = hex_to_u32(tokens[j]);
        for (int j = 0; j < 8; j++) key[j] = hex_to_u32(tokens[4 + j]);
        for (int j = 0; j < 4; j++) expected[j] = hex_to_u32(tokens[12 + j]);

        aradi_encrypt_shuffle(pt, key, encrypted);

        total++;
        if (block_equals(encrypted, expected)) {
            printf("✅ Block %d passed.\n", block_id);
            passed++;
        } else {
            printf("❌ Block %d failed.\n", block_id);
            printf("  Expected: ");
            for (int j = 0; j < 4; j++) printf("%08x ", expected[j]);
            printf("\n  Obtained: ");
            for (int j = 0; j < 4; j++) printf("%08x ", encrypted[j]);
            printf("\n");
        }

        // Optional decryption:
        aradi_decrypt_shuffle(encrypted, key, decrypted);
        if (!block_equals(pt, decrypted)) {
            printf("  ❌ Decryption failed on block %d\n", block_id);
            printf("    Original: ");
            for (int j = 0; j < 4; j++) printf("%08x ", pt[j]);
            printf("\n    Decrypted: ");
            for (int j = 0; j < 4; j++) printf("%08x ", decrypted[j]);
            printf("\n");
        }

        block_id++;
    }

    fclose(fp);
    printf("\nSummary: %d/%d blocks encrypted correctly.\n", passed, total);
}

int main() {
    test_from_csv("aradi_simple_dataset.csv");
    return 0;
}