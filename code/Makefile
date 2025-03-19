# Compiler and flags
CC = gcc
CFLAGS = -fopenmp

# Source files
SOURCES = $(wildcard *.c)

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Rule to compile object files
%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

# Compile only target
compile: $(OBJECTS)

# Clean target
clean:
	rm -f $(OBJECTS)

# Phony targets to prevent conflicts with files named 'compile' or 'clean'
.PHONY: compile clean