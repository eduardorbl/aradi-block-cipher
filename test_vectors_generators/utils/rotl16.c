#include <stdio.h>
#include <stdint.h>
#include "utils.h"

int main() {
    FILE *fp = fopen("../../test_vectors/utils/rotl16_vectors.csv", "w");
    if (!fp) return 1;

    fprintf(fp, "input,n,output\n");

    for (int t = 0; t < 1000; ++t) {
        uint16_t input = rand() & 0xFFFF;
        uint8_t n = rand() % 16;
        uint16_t output = rotl16(input, n);
        fprintf(fp, "%04x,%u,%04x\n", input, n, output);
    }

    fclose(fp);
    return 0;
}
