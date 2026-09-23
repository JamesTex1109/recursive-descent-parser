# =============================================================================
# Title       : Makefile
# Description : Build script for the Cooke Syntax Parser
# Author      : James Carrington Jr. 
# Date        : 04/02/2026
# Usage       : 'make' to build, 'make clean' to remove binaries
# =============================================================================

CC = gcc
CFLAGS = -g -Wall
OBJ = front.o parser.o

# The primary goal is now the parser 
all: cooke_parser

# Link front.o and parser.o into the final executable 
cooke_parser: $(OBJ)
	$(CC) $(CFLAGS) -o cooke_parser $(OBJ)

# Compile front.c into front.o 
front.o: front.c front.h
	$(CC) $(CFLAGS) -c front.c

# Compile parser.c into parser.o 
parser.o: parser.c parser.h front.h
	$(CC) $(CFLAGS) -c parser.c

# Clean up the folder 
clean:
	rm -f *.o cooke_parser