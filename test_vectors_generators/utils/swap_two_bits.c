#include <stdio.h>
#include <stdint.h>
#include "utils.h"

int main() {
    FILE *fp = fopen("../../test_vectors/utils/swap_two_bits_vectors.csv", "w");
    if (!fp) return 1;

    fprintf(fp, "input,output\n");

    for (int t = 0; t < 1000; ++t) {
        uint32_t input = rand();
        uint32_t output = swap_two_bits(input);
        fprintf(fp, "%08x,%08x\n", input, output);
    }

    fclose(fp);
    return 0;
}
