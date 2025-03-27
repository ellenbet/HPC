# This Makefile was originally made using ChatGPT
# Modified for C (instead of C++) 

export C_INCLUDE_PATH = /opt/homebrew/include:/Library/Developer/CommandLineTools/usr/include

# Compiler settings
CC = gcc # Use GCC for C compilation
CFLAGS = -std=c11 -Wall -I/opt/homebrew/include -I/Library/Developer/CommandLineTools/usr/include
#CFLAGS += -I/opt/homebrew/Cellar/gcc@12/12.4.0/lib/gcc/12/gcc/aarch64-apple-darwin21/12/include
#CFLAGS += -I/opt/homebrew/Cellar/gcc@12/12.4.0/lib/gcc/12/gcc/aarch64-apple-darwin21/12/include-fixed
# -fopenmp -I/opt/homebrew/opt/libomp/include  # Uncomment if needed

# Linker flags (adjust for Mac M1 or other systems)
ifeq ($(shell uname), Darwin)
	LDFLAGS = -L/opt/homebrew/lib -larmadillo -lblas -llapack
	LDFLAGS += -L/opt/homebrew/opt/libomp/lib -lomp
	CPPFLAGS += -I/opt/homebrew/opt/libomp/include 
else
	LDFLAGS = -L/usr/local/lib -larmadillo -lblas -llapack
	LDFLAGS += -L/usr/lib/gcc/x86_64-linux-gnu/9 -lgomp
	CPPFLAGS += -I/usr/local/include
endif

# Directories
SRCDIR = src
INCDIR = include
BINDIR = bin

# Main source file (expects a .c file)
MAIN_SRC ?= main.c

# Filenames
SRCFILE = $(basename $(MAIN_SRC))
TARGET = $(BINDIR)/$(SRCFILE)
MAIN_OBJECT = $(BINDIR)/$(SRCFILE).o
SRC_OBJECTS = $(wildcard $(SRCDIR)/*.c)
SRC_OBJECTS := $(patsubst $(SRCDIR)/%.c,$(BINDIR)/%.o,$(SRC_OBJECTS))

# Default build target
all: $(TARGET)

# Linking final executable
$(TARGET): $(MAIN_OBJECT) $(SRC_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile main file
$(MAIN_OBJECT): $(MAIN_SRC)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Compile source files
$(BINDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Run the compiled program with optional arguments
run: $(TARGET)
	@echo "Running program with arguments: $(ARGS)"
	./$(TARGET) $(ARGS)

# Cleanup
clean:
	rm -rf $(BINDIR)/*

# Makefile phony targets
.PHONY: all clean run