#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "utils.h"

int main() {
    FILE *fp = fopen("../../test_vectors/utils/m928_vectors.csv", "w");
    if (!fp) return 1;

    fprintf(fp, "x,y,out_x,out_y\n");

    for (int t = 0; t < 1000; ++t) {
        uint32_t x = rand();
        uint32_t y = rand();
        u32pair_t result = m928(x, y);
        fprintf(fp, "%08x,%08x,%08x,%08x\n", x, y, result.x, result.y);
    }

    fclose(fp);
    return 0;
}
