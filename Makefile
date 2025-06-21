.PHONY= all clean create-dir
CC = gcc
CFLAGS = -Wall -pthread
OUT_DIR = out

CLIENT_SRC := $(wildcard client/*.c)
SERVER_SRC := $(wildcard server/*.c)

all: create-dir $(OUT_DIR)/server $(OUT_DIR)/client
	@echo "Build complete. Nothing new to compile."

$(OUT_DIR)/server:$(SERVER_SRC)
	@echo "SERVER_SRC = [$(SERVER_SRC)]"
	$(CC) $(CFLAGS) -o $@ $(SERVER_SRC)

$(OUT_DIR)/client:$(CLIENT_SRC)
	@echo "CLIENT_SRC = [$(CLIENT_SRC)]"
	$(CC) $(CFLAGS) -o $@ $(CLIENT_SRC)

create-dir:
	@mkdir -p $(OUT_DIR)

clean:
	@rm -rf $(OUT_DIR)
	@echo "Clean Success"

