#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "utils.h"

int main() {
    FILE *fp = fopen("test_vectors/utils/rotl32_vectors.csv", "w");
    if (!fp) return 1;

    fprintf(fp, "input,n,output\n");

    for (int t = 0; t < 1000; ++t) {
        uint32_t input = rand();
        uint8_t n = rand() % 32;
        uint32_t output = rotl32(input, n);
        fprintf(fp, "%08x,%u,%08x\n", input, n, output);
    }

    fclose(fp);
    return 0;
}
