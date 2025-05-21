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
	rm -f $(TARGET) src/*.o

# Executar o teste
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean
