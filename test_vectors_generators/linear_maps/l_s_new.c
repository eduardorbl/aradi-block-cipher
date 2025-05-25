#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "linear_maps.h"
#include "tables.h"

int main() {
    FILE *fp = fopen("test_vectors/linear_maps/l_s_new_vectors.csv", "w");
    if (!fp) return 1;

    fprintf(fp, "U_in,L_in,j,U_out,L_out\n");

    for (int t = 0; t < 1000; ++t) {
        uint32_t U = rand();
        uint32_t L = rand();
        uint8_t j = rand() % 4;

        uint32_t U_orig = U;
        uint32_t L_orig = L;

        l_s_new(ARADI_A_S_2, ARADI_B_S_2, ARADI_C_S_2, j, &U, &L);

        fprintf(fp, "%08x,%08x,%u,%08x,%08x\n", U_orig, L_orig, j, U, L);
    }

    fclose(fp);
    return 0;
}
