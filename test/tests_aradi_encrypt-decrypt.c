#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "aradi.h" 
#include "keyschedule.h"

// Função auxiliar para imprimir vetores de 32 bits
void print_block(const char* label, const uint32_t block[4]) {
    printf("%s: ", label);
    for (int i = 0; i < 4; ++i)
        printf("%08x ", block[i]);
    printf("\n");
}

// Gera vetor aleatório de 128 bits (4 x uint32_t)
void random_block(uint32_t block[4]) {
    for (int i = 0; i < 4; ++i)
        block[i] = ((uint32_t)rand() << 16) | (rand() & 0xFFFF);
}

// Gera chave aleatória de 256 bits (8 x uint32_t)
void random_key(uint32_t key[8]) {
    for (int i = 0; i < 8; ++i)
        key[i] = ((uint32_t)rand() << 16) | (rand() & 0xFFFF);
}

// Verifica se dois blocos de 4 palavras são iguais
int blocks_equal(const uint32_t a[4], const uint32_t b[4]) {
    return memcmp(a, b, 4 * sizeof(uint32_t)) == 0;
}

int main() {
    srand((unsigned int)time(NULL)); // Inicializa o gerador de números aleatórios

    const int N = 1000;
    int errors = 0;

    for (int i = 0; i < N; ++i) {
        uint32_t plaintext[4], ciphertext[4], decrypted[4];
        uint32_t key[8];

        random_block(plaintext);
        random_key(key);

        // Escolha entre shuffle ou não shuffle:
        aradi_encrypt_shuffle(plaintext, key, ciphertext);
        aradi_decrypt_shuffle(ciphertext, key, decrypted);

        if (!blocks_equal(plaintext, decrypted)) {
            printf("❌ Erro no teste %d\n", i);
            print_block("Plaintext", plaintext);
            print_block("Ciphertext", ciphertext);
            print_block("Decrypted", decrypted);
            ++errors;
        }
    }

    if (errors == 0)
        printf("✅ Todos os %d testes passaram com sucesso!\n", N);
    else
        printf("⚠️ %d de %d testes falharam.\n", errors, N);

    return 0;
}
