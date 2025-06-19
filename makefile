CC = clang
CFLAGS = -std=c99 -g -Wall -Wextra
LDFLAGS = 

SRC_DIR = src
BUILD_DIR = build
LIB_DIR = lib
TEST_DIR = tests

# All source files, recursively
SRC_FILES = \
	$(wildcard $(SRC_DIR)/*.c) \
	$(wildcard $(SRC_DIR)/**/*.c) \
	$(wildcard $(SRC_DIR)/**/**/*.c) \
	$(wildcard $(SRC_DIR)/**/**/**/*.c)

# Filter out the regular main
SRC_NO_MAIN = $(filter-out $(SRC_DIR)/main.c, $(SRC_FILES))

LIB_FILES = $(wildcard $(LIB_DIR)/utils_c/src/*.c)

APP_BIN = $(BUILD_DIR)/app
TEST_BIN = $(BUILD_DIR)/test

all: $(APP_BIN)

$(APP_BIN): $(SRC_FILES) $(LIB_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
run:
	@cd $(BUILD_DIR) && ./app


test: $(TEST_BIN)
	@cd $(BUILD_DIR) && ./test

$(TEST_BIN): $(SRC_NO_MAIN) $(LIB_FILES) $(TEST_DIR)/main.c $(wildcard $(TEST_DIR)/*.c)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean run test

