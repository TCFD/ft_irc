# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror --std=c++98 -I./includes -g3

# Directories
SRC_DIR = .
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
LIB_DIR = $(BUILD_DIR)/lib
BIN_DIR = $(BUILD_DIR)/bin

# Find all .cpp files except main.cpp
SRCS = $(shell find $(SRC_DIR) -name "*.cpp" ! -name "main.cpp")
# Generate corresponding .o files in the obj directory
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Output library and executable
LIB = $(LIB_DIR)/lib.a
EXEC = $(BIN_DIR)/exec

# Default target
all: $(LIB)

# Create the static library
$(LIB): $(OBJS)
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $^

# Rule to build the executable by compiling main.cpp and linking with the static library
compile: $(EXEC)

# Compile main.cpp and link with the static library to create the executable
$(EXEC): $(LIB) $(OBJ_DIR)/main.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)/main.o $(LIB) -o $@

# Compile main.cpp to main.o
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile .cpp files to .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build directory
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all clean compile
