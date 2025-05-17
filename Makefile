CC = gcc
CFLAGS = $(shell pkg-config --cflags raylib)
LDFLAGS = $(shell pkg-config --libs raylib)

FILE?=main.c

# Specifically for macOS
ifeq ($(shell uname), Darwin)
    LDFLAGS += -framework IOKit -framework Cocoa -framework OpenGL
endif

# I want to do a raylib file.c and execute the following command:
# Usage example: `make compile FILE=main.c OUTPUT=play`
compile: $(FILE)
	$(CC) -o $(OUTPUT) $^ $(CFLAGS) $(LDFLAGS)
