#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "linear_maps.h"
#include "tables.h"

int main() {
    FILE *fp = fopen("../../test_vectors/linear_maps/lm_new_vectors.csv", "w");
    if (!fp) return 1;

    fprintf(fp, "z_in,j,z_out\n");

    for (int t = 0; t < 1000; ++t) {
        uint32_t z = rand();
        uint8_t j = rand() % 4;

        uint32_t z_out = lm_new(ARADI_A, ARADI_B, ARADI_D, j, z);

        fprintf(fp, "%08x,%u,%08x\n", z, j, z_out);
    }

    fclose(fp);
    return 0;
}
