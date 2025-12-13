CXX := g++

# Optional: set SFML_DIR to point to your SFML 2 installation root
# Example: make SFML_DIR="C:/SFML-2.5.1"
SFML_DIR ?=
SFML_INC := $(if $(SFML_DIR),-I$(SFML_DIR)/include,)
SFML_LIB := $(if $(SFML_DIR),-L$(SFML_DIR)/lib,)

# Compiler and linker flags
CXXFLAGS := -std=c++17 -Iinclude -O2 $(SFML_INC)
LDFLAGS := $(SFML_LIB) -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lbox2d

SRCS := $(wildcard src/*.cpp)
TARGET := bin/circus

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
