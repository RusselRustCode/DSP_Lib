CC      = clang-15 # Компилятор будет Clang(LLVM)
LD      = riscv64-linux-gnu-gcc #Линкер будет GCC

# Флаги компиляции (Clang)
CFLAGS  = -std=c99 -O2 -Iinclude \
          --target=riscv64-linux-gnu \
          -march=rv64gcv \
          -mabi=lp64d \
          --sysroot=/usr/riscv64-linux-gnu

# Флаги линковки (GCC)
LDFLAGS = -march=rv64gcv -mabi=lp64d -static

TARGET  = dsp_demo

SRC_DIR = src
EXAMPLES_DIR = examples

SRCS    = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(EXAMPLES_DIR)/*.c)
OBJS    = $(SRCS:.c=.o)


$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^ -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean