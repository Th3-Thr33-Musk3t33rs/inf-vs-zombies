CC = gcc
CFLAGS = $(shell pkg-config --cflags raylib) -Iinclude
LDFLAGS = $(shell pkg-config --libs raylib)

SRC_DIR := src
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:.c=.o)

OUTPUT ?= a.out

# macOS frameworks
ifeq ($(shell uname), Darwin)
    LDFLAGS += -framework IOKit -framework Cocoa -framework OpenGL
endif

# Default target: compile all and link
all: $(OUTPUT)

# Link objects into executable
$(OUTPUT): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

# Compile source filesc
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Run the game and clean executable
run: all
	./$(OUTPUT)
	rm -f $(OUTPUT)

# Clean all object files and executable
clean:
	rm -f $(OUTPUT) $(OBJS)

.PHONY: all run clean
