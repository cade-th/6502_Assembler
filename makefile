CC = clang
CFLAGS = -std=c99 -g -Wall -Wextra
LDFLAGS = 

SRC_DIR = src
BUILD_DIR = build
LIB_DIR = lib

SRC_FILES = \
	$(wildcard src/*.c) \
	$(wildcard src/**/*.c) \
	$(wildcard src/**/**/*.c) \
	$(wildcard src/**/**/**/*.c)

LIB_FILES = $(wildcard $(LIB_DIR)/utils_c/src/*.c)

APP_BIN = $(BUILD_DIR)/app

all: $(APP_BIN)

$(APP_BIN): $(SRC_FILES) $(LIB_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
clean:
	rm -rf $(BUILD_DIR)

run:
	@cd build && ./app

.PHONY: all clean

