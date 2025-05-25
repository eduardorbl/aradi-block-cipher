#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "linear_maps.h"
#include "tables.h"

int main() {
    FILE *fp = fopen("test_vectors/linear_maps/lm_s_nsa_vectors.csv", "w");
    if (!fp) return 1;

    fprintf(fp, "z_in,j,z_out\n");

    const uint8_t C[4] = {29 ,23 ,29 ,15};

    for (int t = 0; t < 1000; ++t) {
        uint32_t z = rand();
        uint8_t j = rand() % 4;

        uint32_t z_out = lm_s_nsa(ARADI_A, ARADI_B, C, j, z);

        fprintf(fp, "%08x,%u,%08x\n", z, j, z_out);
    }

    fclose(fp);
    return 0;
}
