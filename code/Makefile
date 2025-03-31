# Compiler and flags
CC = gcc
CFLAGS = -fopenmp
TEST_CFLAGS = -DTEST

# Source files
SOURCES = $(wildcard *.c)

OBJECTS= $(SOURCES:.c=)

# Rule to compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile only target
compile: $(OBJECTS)

# Clean target
clean:
	rm -f $(OBJECTS)

test: CFLAGS += $(TEST_CFLAGS)
test: compile
	./run-tests.sh

# Phony targets to prevent conflicts with files named 'compile' or 'clean'
.PHONY: compile clean test
