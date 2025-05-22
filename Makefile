CC = gcc
CFLAGS = $(shell pkg-config --cflags raylib)
LDFLAGS = $(shell pkg-config --libs raylib)

FILE ?= src/game.c
OUTPUT ?= a.out

# Specifically for macOS.
ifeq ($(shell uname), Darwin)
    LDFLAGS += -framework IOKit -framework Cocoa -framework OpenGL
endif

# Compile the code.
compile: $(FILE)
	$(CC) -o $(OUTPUT) $^ $(CFLAGS) $(LDFLAGS)

# Compile, run and clean.
run: compile
	./$(OUTPUT)
	rm -f $(OUTPUT)
