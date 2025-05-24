#include <stdio.h>
#include <stdint.h>
#include "keyschedule.h"

int main() {
    FILE *fp = fopen("../../test_vectors/keyschedule/subkeys_shuffle_vectors.csv", "w");
    if (!fp) return 1;

    fprintf(fp, "k0,k1,k2,k3,k4,k5,k6,k7");
    for (int i = 0; i < 17; ++i) for (int j = 0; j < 4; ++j) fprintf(fp, ",rk%d%d", i, j);
    fprintf(fp, "\n");

    for (int t = 0; t < 100; ++t) {
        uint32_t key[8], rk[17][4];
        for (int i = 0; i < 8; ++i) key[i] = rand();

        subkeys_shuffle(key, rk);

        for (int i = 0; i < 8; ++i) fprintf(fp, "%08x,", key[i]);
        for (int i = 0; i < 17; ++i)
            for (int j = 0; j < 4; ++j)
                fprintf(fp, "%08x%s", rk[i][j], (i == 16 && j == 3) ? "\n" : ",");
    }

    fclose(fp);
    return 0;
}
