# ==============================
# Настройки проекта
# ==============================
CC      = gcc
CFLAGS  = -std=c99 -Wextra -O2 -Iinclude
TARGET  = dsp_demo.exe

# Источники: все .c в src/ + примеры (ли нужно)
SRC_DIR = src
EXAMPLES_DIR = examples

# Собираем все .c из src и examples (можно убрать examples, если не нужен)
SRCS    = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(EXAMPLES_DIR)/*.c)
OBJS    = $(SRCS:.c=.o)

# ==============================
# Правила
# ==============================
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean