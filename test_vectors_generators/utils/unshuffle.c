#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "utils.h"

int main() {
    FILE *fp = fopen("../../test_vectors/utils/unshuffle_vectors.csv", "w");
    if (!fp) return 1;

    fprintf(fp, "input,output\n");

    for (int t = 0; t < 1000; ++t) {
        uint32_t input = rand();
        uint32_t output = unshuffle(input);
        fprintf(fp, "%08x,%08x\n", input, output);
    }

    fclose(fp);
    return 0;
}
