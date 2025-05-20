# ARADI Block Cipher (C Implementation)

Este projeto contém uma implementação em C da cifra de blocos **ARADI**, baseada no material da disciplina MC938 — Criptografia e Segurança de Dados (UNICAMP).

A implementação atual **correspondente ao material de referência**, ou seja, **inclui as otimizações por shuffle bit-level**.

## 📁 Estrutura
├── include/  
│ ├── aradi.h  
│ ├── keyschedule.h  
│ ├── linear_maps.h  
│ ├── sbox.h  
│ ├── tables.h  
│ └── utils.h  
├── src/  
│ ├── aradi.c  
│ ├── keyschedule.c  
│ ├── linear_maps.c  
│ ├── sbox.c  
│ ├── tables.c  
│ └── utils.c  
├── test/  
│ ├── test_aradi.c   
│ ├── aradi_test_from_csv.c   
│ ├── test_utils.c  
│ └── tests_aradi_encrypt-decrypt.c

## 🧪 Testes

Os testes verificam se:

- O texto cifrado bate com o esperado (dos slides/artigo original)
- A função de decifragem inverte a cifragem corretamente
- Se o ARADI com shuffle está funcionando de acordo com a implementação do slide (dataset em .csv).
- Se a decriptação está funcionando.

Para compilar (modifique o teste alvo no arquivo Makefile):

```bash
make
./executavel_teste

