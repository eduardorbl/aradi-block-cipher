CC = clang
CFLAGS = -Wall -Wextra -Iinclude -O2
SRC = src/aradi.c src/linear_maps.c src/utils.c src/sbox.c src/keyschedule.c src/tables.c
OBJ = $(SRC:.c=.o)
TEST = test/test_aradi.c
TARGET = test_aradi

all: $(TARGET)

$(TARGET): $(OBJ) $(TEST)
	$(CC) $(CFLAGS) $(OBJ) $(TEST) -o $(TARGET)
	rm -f src/*.o

clean:
	rm -f $(TARGET) src/*.o

.PHONY: all clean

