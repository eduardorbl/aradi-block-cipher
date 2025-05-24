#include <stdio.h>
#include <stdint.h>
#include "utils.h"

int main() {
    FILE *fp = fopen("../../test_vectors/utils/shuffle_vectors.csv", "w");
    if (!fp) return 1;

    fprintf(fp, "input,output\n");

    for (int t = 0; t < 1000; ++t) {
        uint32_t input = rand();
        uint32_t output = shuffle(input);
        fprintf(fp, "%08x,%08x\n", input, output);
    }

    fclose(fp);
    return 0;
}
