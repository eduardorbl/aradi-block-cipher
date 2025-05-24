#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "sbox.h"

int main() {
    FILE *fp = fopen("../../test_vectors/sbox/sbox_inverse_vectors.csv", "w");
    if (!fp) return 1;

    fprintf(fp, "w_in,x_in,y_in,z_in,w_out,x_out,y_out,z_out\n");

    for (int t = 0; t < 1000; ++t) {
        uint32_t w = rand();
        uint32_t x = rand();
        uint32_t y = rand();
        uint32_t z = rand();

        uint32_t w_out = w;
        uint32_t x_out = x;
        uint32_t y_out = y;
        uint32_t z_out = z;

        sbox_inverse(&w_out, &x_out, &y_out, &z_out);

        fprintf(fp, "%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x\n",
                w, x, y, z, w_out, x_out, y_out, z_out);
    }

    fclose(fp);
    return 0;
}
