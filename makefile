# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lbox2d

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Target
TARGET = $(BIN_DIR)/CircusCharlie.exe

# Sources
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	@echo "Linking..."
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete!"

# Compile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directories
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

# Run
run: $(TARGET)
	cd $(BIN_DIR) && ./CircusCharlie.exe

# Rebuild
rebuild: clean all

.PHONY: all clean run rebuild
