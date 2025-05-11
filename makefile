CC = gcc
CFLAGS = -Wall -Wextra -g -m64
INCLUDES = -Iresources
SRC = $(wildcard src/*.c)
BIN = bin
TARGET = $(BIN)/main

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p $(BIN)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

clean:
	rm -rf $(BIN)

.PHONY: all clean