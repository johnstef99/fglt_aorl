SRC_DIR  = ./src
OBJ_DIR  = ./obj
LIB_DIR  = ./libs
BIN_DIR  = ./bin

CFILES   = $(wildcard $(SRC_DIR)/*.c)
LIBFILES = $(wildcard $(LIB_DIR)/*.c)
OBJFILES = $(CFILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o) $(LIBFILES:$(LIB_DIR)/%.c=$(OBJ_DIR)/%.o)
OUT      = $(BIN_DIR)/fglt_aorl

CC      = clang
CFLAGS  = -Wall -fopencilk
LDFLAGS = -fopencilk

ifeq ($(shell uname),Darwin)
	# config compile on macos
	CC  = xcrun /opt/opencilk/bin/clang
else
	# config compile on HPC AUTH
	CC  = $(OPENCILK)/build/bin/clang
endif

all: CFLAGS += -O3
all: $(OUT)

debug: CFLAGS += -DDEBUG
debug: $(OUT)

cilkscan: CFLAGS  += -Og -g
cilkscan: LDFLAGS += -fsanitize=cilk -D_FORTIFY_SOURCE=0
cilkscan: clean $(OUT) run

$(OUT): $(OBJFILES)
	$(CC) $(LDFLAGS) -o $@ $^ 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(LIB_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean test run
run:
	./bin/fglt_aorl assets/mine.mtx

clean:
	rm -f $(OBJ_DIR)/* $(BIN_DIR)/*
