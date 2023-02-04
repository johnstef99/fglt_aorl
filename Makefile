SRC_DIR  = ./src
OBJ_DIR  = ./obj
LIB_DIR  = ./libs
BIN_DIR  = ./bin

CC       = clang
CFILES   = $(wildcard $(SRC_DIR)/*.c)
LIBFILES   = $(wildcard $(LIB_DIR)/*.c)
OBJFILES = $(CFILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o) $(LIBFILES:$(LIB_DIR)/%.c=$(OBJ_DIR)/%.o)
OUT      = $(BIN_DIR)/fglt_aorl

CFLAGS = -Wall -O3

$(OUT): $(OBJFILES)
	$(CC) -o $@ $^ 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(LIB_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean test run
run:
	./bin/fglt_aorl assets/mine.mtx

clean:
	rm -f $(OBJ_DIR)/* $(BIN_DIR)/*
