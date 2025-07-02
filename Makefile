CC = gcc
CFLAGS = $(shell pkg-config --cflags raylib) -Iinclude
LDFLAGS = $(shell pkg-config --libs raylib)

SRC_DIR := src
# SRCS : Lista de todos os arquivos (.c), separados por espaço.
SRCS := $(wildcard $(SRC_DIR)/*.c)
# OBJS : Pega a lista de SRCS e substitui '.c' por '.o'.
OBJS := $(SRCS:.c=.o)

OUTPUT ?= memes-vs-zombies.out

# Requisitos de frameworks para compilar na arquitetura Darwin do mac.
ifeq ($(shell uname), Darwin)
    LDFLAGS += -framework IOKit -framework Cocoa -framework OpenGL
endif

# Compila tudo e linka.
all: $(OUTPUT)

# Linka os objetos com os seus respectivos executáveis.
# $^ : Todas as dependências separadas por espaços.
$(OUTPUT): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

# Compila os source files.
# $< : Nome da primeira dependência.
# $@ : Nome do primeiro alvo.
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Compila e roda o jogo e depois limpa os executáveis.
run: all
	./$(OUTPUT)
	rm -f $(OBJS)

# Limpa os executáveis deixados pra trás caso existam.
clean:
	rm -f $(OUTPUT) $(OBJS)

.PHONY: all run clean
