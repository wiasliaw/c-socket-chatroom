INC_DIR := inc
SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

CC := gcc
CFLAGS := -Wall -I$(INC_DIR)
LINKS := -lpthread

INCS_FILES = $(wildcard $(INC_DIR)/*.h)
SRCS_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJS_FILES = $(SRCS_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

main:	client server

client: $(OBJS_FILES)
	@mkdir -p $(BIN_DIR)
	gcc $(OBJ_DIR)/client.o -o $(BIN_DIR)/client $(LINKS)

server: $(OBJS_FILES)
	@mkdir -p $(BIN_DIR)
	gcc $(OBJ_DIR)/server.o -o $(BIN_DIR)/server

clean:
	-rm $(OBJ_DIR)/*.o $(TARGET_FILES)
	-rm $(BIN_DIR)/*

.PHONY: client server clean
