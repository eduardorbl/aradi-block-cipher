#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "aradi.h"

int main() {
    FILE *fp = fopen("test_vectors/aradi/aradi_decrypt_shuffle_vectors.csv", "w");
    if (!fp) return 1;

    fprintf(fp, "in0,in1,in2,in3,k0,k1,k2,k3,k4,k5,k6,k7,out0,out1,out2,out3\n");

    for (int t = 0; t < 100; ++t) {
        uint32_t ciphertext[4], key[8], output[4];
        for (int i = 0; i < 4; ++i) ciphertext[i] = rand();
        for (int i = 0; i < 8; ++i) key[i] = rand();

        aradi_decrypt_shuffle(ciphertext, key, output);

        for (int i = 0; i < 4; ++i) fprintf(fp, "%08x,", ciphertext[i]);
        for (int i = 0; i < 8; ++i) fprintf(fp, "%08x,", key[i]);
        for (int i = 0; i < 4; ++i) fprintf(fp, "%08x%s", output[i], (i == 3) ? "\n" : ",");
    }

    fclose(fp);
    return 0;
}
