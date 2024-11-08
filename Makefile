# Makefile for sudoku.cpp

# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -O3 -std=c++17
LDFLAGS := -ltbb  # Link the Intel TBB library

# Target executable
TARGET := sudoku

# Source files
SRC := sudoku.cpp

# Object files
OBJ := $(SRC:.cpp=.o)

# Default target to build the executable
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJ) $(TARGET)

# Phony targets (to avoid conflicts with file names)
.PHONY: all clean