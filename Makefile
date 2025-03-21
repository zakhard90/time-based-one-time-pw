SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj

$(shell mkdir -p $(BIN_DIR) $(OBJ_DIR))

CC = gcc
CFLAGS = -Wall -Wextra -O2

LDFLAGS = -lssl -lcrypto

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
TARGET = $(BIN_DIR)/totp

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

.PHONY: all clean