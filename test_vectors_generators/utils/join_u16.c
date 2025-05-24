#include <stdio.h>
#include <stdint.h>
#include "utils.h"

int main() {
    FILE *fp = fopen("../../test_vectors/utils/join_u16_vectors.csv", "w");
    if (!fp) return 1;

    fprintf(fp, "high,low,output\n");

    for (int t = 0; t < 1000; ++t) {
        uint16_t high = rand() & 0xFFFF;
        uint16_t low = rand() & 0xFFFF;
        uint32_t output = join_u16(high, low);
        fprintf(fp, "%04x,%04x,%08x\n", high, low, output);
    }

    fclose(fp);
    return 0;
}
