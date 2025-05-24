#include <stdio.h>
#include <stdint.h>
#include "utils.h"

int main() {
    FILE *fp = fopen("../../test_vectors/utils/split_u32_vectors.csv", "w");
    if (!fp) return 1;

    fprintf(fp, "input,high,low\n");

    for (int t = 0; t < 1000; ++t) {
        uint32_t input = rand();
        uint16_t high, low;
        split_u32(input, &high, &low);
        fprintf(fp, "%08x,%04x,%04x\n", input, high, low);
    }

    fclose(fp);
    return 0;
}
