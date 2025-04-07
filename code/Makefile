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

serial: compile
	chmod +x run-multiple-omp.sh
	./run-multiple-omp.sh serial

omp: compile
	chmod +x run-multiple-omp.sh
	./run-multiple-omp.sh omp

all: serial omp

# Clean target
clogs:
	rm -f omp_*.e* omp_*.o* serial_*.e* serial_*.o*

ctests:
	rm -f tests/* 

clean: ctests clogs
	rm -f $(OBJECTS)

test:
	./run-tests.sh

# Phony targets to prevent conflicts with files named 'compile' or 'clean'
.PHONY: compile clean