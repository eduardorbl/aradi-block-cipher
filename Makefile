# Compilador e flags
CC = clang
CFLAGS = -Wall -Wextra -Iinclude -O2

# Fontes principais
SRC = src/aradi.c src/linear_maps.c src/utils.c src/sbox.c src/keyschedule.c src/tables.c
OBJ = $(SRC:.c=.o)

# Código de teste
TEST = test/test_aradi.c
TARGET = executavel_teste

# Regra principal
all: $(TARGET)

$(TARGET): $(OBJ) $(TEST)
	$(CC) $(CFLAGS) $(OBJ) $(TEST) -o $(TARGET)

# Limpeza
clean:
	rm -f $(TARGET) src/*.o test_vectors_generators/aradi/*.o \
	test_vectors_generators/keyschedule/*.o test_vectors_generators/linear_maps/*.o \
	test_vectors_generators/sbox/*.o test_vectors_generators/utils/*.o \
	test_vectors_generators/aradi/*.o test_vectors_generators/keyschedule/*.o \
	test_vectors_generators/linear_maps/*.o test_vectors_generators/sbox/*.o \
	test_vectors_generators/utils/*.o

# Executar o teste
run: $(TARGET)
	./$(TARGET)

# Diretórios dos geradores
VECTOR_DIRS = test_vectors_generators/aradi \
              test_vectors_generators/keyschedule \
              test_vectors_generators/linear_maps \
              test_vectors_generators/sbox \
              test_vectors_generators/utils

# Arquivos dos geradores
GENERATORS = \
	test_vectors_generators/aradi/aradi_decrypt_shuffle_2blocks.c \
	test_vectors_generators/aradi/aradi_decrypt_shuffle.c \
	test_vectors_generators/aradi/aradi_decrypt.c \
	test_vectors_generators/aradi/aradi_encrypt_shuffle_2blocks.c \
	test_vectors_generators/aradi/aradi_encrypt_shuffle.c \
	test_vectors_generators/aradi/aradi_encrypt.c \
	test_vectors_generators/keyschedule/derive_subkeys.c \
	test_vectors_generators/keyschedule/subkeys_shuffle.c \
	test_vectors_generators/linear_maps/l_s_new.c \
	test_vectors_generators/linear_maps/lm_new.c \
	test_vectors_generators/linear_maps/lm_s_new.c \
	test_vectors_generators/linear_maps/lm_s_nsa.c \
	test_vectors_generators/linear_maps/lm.c \
	test_vectors_generators/sbox/sbox_inverse.c \
	test_vectors_generators/sbox/sbox_layer.c \
	test_vectors_generators/utils/join_u16.c \
	test_vectors_generators/utils/m13.c \
	test_vectors_generators/utils/m928.c \
	test_vectors_generators/utils/rotl16.c \
	test_vectors_generators/utils/rotl32.c \
	test_vectors_generators/utils/shuffle.c \
	test_vectors_generators/utils/split_u32.c \
	test_vectors_generators/utils/swap_two_bits.c \
	test_vectors_generators/utils/unshuffle.c

# Objetos dos geradores
GENERATOR_OBJS = $(GENERATORS:.c=)

# Regra para compilar todos os geradores
generate_vectors:
	@echo "Are you sure you want to generate all test vectors? [y/N]"
	@read ans; \
	if [ "$$ans" = "y" ] || [ "$$ans" = "Y" ]; then \
		for gen in $(GENERATORS); do \
			bin=$${gen%.c}; \
			echo "Compiling $$gen..."; \
			$(CC) $(CFLAGS) $$gen $(SRC) -o $$bin; \
			echo "Running $$bin..."; \
			$$bin; \
		done; \
	else \
		echo "Aborted."; \
	fi

.PHONY: all clean run generate_vectors
