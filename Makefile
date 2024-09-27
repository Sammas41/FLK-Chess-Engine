# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic -g

# Source directory
SRC_DIR = src

# Build directory
BUILD_DIR = build

# Executable name
TARGET = chess_engine

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Default target
all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET)

# Rule to create build directory
$(BUILD_DIR):
	mkdir -p $@

# Rule to compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to link object files into executable
$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

# Run the chess engine
run: $(BUILD_DIR)/$(TARGET)
	./$(BUILD_DIR)/$(TARGET)

# Clean rule
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all clean run