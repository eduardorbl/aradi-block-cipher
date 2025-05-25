#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "aradi.h"

#define MAX_TRIALS 100000

void random_input(uint32_t arr[], size_t len) {
    for (size_t i = 0; i < len; ++i) {
        arr[i] = rand();
    }
}

double measure_time(void (*encrypt_fn)(const uint32_t*, const uint32_t*, uint32_t*),
                    const uint32_t key[8], size_t trials) {
    uint32_t input[4], output[4];

    clock_t start = clock();

    for (size_t i = 0; i < trials; ++i) {
        random_input(input, 4);
        encrypt_fn(input, key, output);
    }

    clock_t end = clock();

    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

int main() {
    srand(time(NULL));

    size_t sizes[] = {100, 500, 1000, 5000, 10000, 20000, 50000, 100000};
    size_t num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    FILE *fp = fopen("bench_results.csv", "w");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    fprintf(fp, "Trials,No_Shuffle(s),Shuffle_1Block(s),Shuffle_2Blocks(s)\n");
    printf("Trials,No_Shuffle(s),Shuffle_1Block(s),Shuffle_2Blocks(s)\n");

    uint32_t key[8];
    random_input(key, 8);

    for (size_t i = 0; i < num_sizes; ++i) {
        size_t trials = sizes[i];

        double time_noshuffle = measure_time(aradi_encrypt, key, trials);
        double time_shuffle = measure_time(aradi_encrypt_shuffle, key, trials);
        double time_shuffle2 = measure_time(aradi_encrypt_shuffle_2blocks, key, trials);

        fprintf(fp, "%zu,%.6f,%.6f,%.6f\n", trials, time_noshuffle, time_shuffle, time_shuffle2);
        printf("%zu,%.6f,%.6f,%.6f\n", trials, time_noshuffle, time_shuffle, time_shuffle2);
    }

    fclose(fp);

    return 0;
}
