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
	find . -type f \( -name '*.o' -o -perm +111 \) -exec rm -f {} +

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
BIN_DIR = bin

generate_vectors:
	@echo "\033[31mAre you sure you want to generate all test vectors? (if you do it you will delete all actual vectors) [y/N]\033[0m"
	@read ans; \
	if [ "$$ans" = "y" ] || [ "$$ans" = "Y" ]; then \
		echo "Criando diretórios de test_vectors..."; \
		mkdir -p test_vectors/aradi \
		         test_vectors/keyschedule \
		         test_vectors/linear_maps \
		         test_vectors/sbox \
		         test_vectors/utils; \
		mkdir -p $(BIN_DIR); \
		for gen in $(GENERATORS); do \
			file=$$(basename $$gen .c); \
			echo "Compiling $$gen..."; \
			$(CC) $(CFLAGS) $$gen $(SRC) -o $(BIN_DIR)/$$file; \
		done; \
		for gen in $(GENERATORS); do \
			file=$$(basename $$gen .c); \
			echo "Running $(BIN_DIR)/$$file..."; \
			./$(BIN_DIR)/$$file; \
		done; \
	else \
		echo "Aborted."; \
	fi
	@echo "All test vectors generated."

# Regra para compilar o .csv de benchmarks
# Flags that make No-Shuffle better: clang -Wall -Wextra -Iinclude -O3 -march=native -flto benchmarks/bench_aradi.c src/*.c -o benchmarks/bench_aradi
benchmarks:
	clang -Wall -Wextra -Iinclude benchmarks/bench_aradi.c src/*.c -o benchmarks/bench_aradi
	./benchmarks/bench_aradi > benchmarks/bench_results.csv
	@echo "Benchmarks generated in benchmarks/bench_results.csv"
	@echo "Running Python script to plot benchmarks..."
	. benchmarks/venv/bin/activate && python3 benchmarks/plot_bench.py && deactivate
	@echo "Benchmarks plotted in benchmarks/bench_results.png"

.PHONY: all clean run generate_vectors benchmarks