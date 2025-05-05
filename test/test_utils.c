#include <stdio.h>
#include "utils.h"

int main() {
    uint16_t u = 0x1234, l = 0xABCD;
    uint32_t combined = join_u16(u, l);
    printf("Combined: 0x%08X\n", combined);

    uint16_t uh, ul;
    split_u32(combined, &uh, &ul);
    printf("Split: high = 0x%04X, low = 0x%04X\n", uh, ul);

    uint16_t rotated = rotl16(0x1234, 4);
    printf("Rotated left 4: 0x%04X\n", rotated);

    return 0;
}
// gcc -Iinclude -o test_utils test/test_utils.c src/utils.c
// Run with: ./test_utils
// Expected output:
// Combined: 0x1234ABCD
// Split: high = 0x1234, low = 0xABCD
// Rotated left 4: 0x2341
// Clean up: rm test_utils
