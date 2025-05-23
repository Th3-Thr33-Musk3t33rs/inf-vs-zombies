CC = gcc
CFLAGS = $(shell pkg-config --cflags raylib) -Iinclude
LDFLAGS = $(shell pkg-config --libs raylib)

SRC_DIR := src
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:.c=.o)

OUTPUT ?= a.out

# Requisitos de frameworks para compilar na arquitetura Darwin do mac.
ifeq ($(shell uname), Darwin)
    LDFLAGS += -framework IOKit -framework Cocoa -framework OpenGL
endif

# Compila tudo e linka.
all: $(OUTPUT)

# Linka os objetos com os seus respectivos executáveis.
$(OUTPUT): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

# Compila os source files.
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Compila e roda o jogo e depois limpa os executáveis.
run: all
	./$(OUTPUT)
	rm -f $(OUTPUT) $(OBJS)

# Limpa os executáveis deixados pra trás caso existam.
clean:
	rm -f $(OUTPUT) $(OBJS)

.PHONY: all run clean
