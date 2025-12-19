# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Iinclude

# Find all cpp files inside src/
SRC = $(shell find src -name "*.cpp")

# Output executable name
TARGET = memsim

# Default target
all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Clean compiled files
clean:
	rm -f $(TARGET)
