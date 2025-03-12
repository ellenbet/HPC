
# This Makefile was made using ChatGPT
# Kompilator -> can be changed in command line during make run /make all


# Kompilasjonsflagg
CXXFLAGS = -std=c++17 -Wall -I/opt/homebrew/include 
#-fopenmp -I/opt/homebrew/opt/libomp/include

# Linkeflagg -> this works on mac m1, please test for other systems
ifeq ($(shell uname), Darwin)
	CXX = g++-12
	LDFLAGS = -L/opt/homebrew/lib -larmadillo -lblas -llapack
	LDFLAGS += -L/opt/homebrew/opt/libomp/lib -lomp
	CPPFLAGS += -I/opt/homebrew/opt/libomp/include 

else
	CXX = g++
	LDFLAGS = -L/usr/local/lib -larmadillo -lblas -llapack
	LDFLAGS += -L/usr/lib/gcc/x86_64-linux-gnu/9 -lgomp
	CPPFLAGS += -I/usr/local/include
endif

# Mapper
SRCDIR = src
INCDIR = include
BINDIR = bin

# Hovedkilden (kan settes fra kommandolinjen, forventer en .cpp fil)
MAIN_SRC ?= main.cpp

# Filnavnene
SRCFILE = $(basename $(MAIN_SRC))
TARGET = $(BINDIR)/$(SRCFILE)
MAIN_OBJECT = $(BINDIR)/$(SRCFILE).o
SRC_OBJECTS = $(wildcard $(SRCDIR)/*.cpp)
SRC_OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(SRC_OBJECTS))

# Standardregel
all: $(TARGET)

# Sammensetting
$(TARGET): $(MAIN_OBJECT) $(SRC_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Kompileringsregel for hovedfilene
$(MAIN_OBJECT): $(MAIN_SRC)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Kompileringsregel for src-filene
$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@(mkdir -p $(BINDIR))
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Regel for å kjøre programmet
run: $(TARGET)
	./$(TARGET)

# Rense opp
clean:
	rm -rf $(BINDIR)/*

# Bygging og kjøring
.PHONY: all clean run