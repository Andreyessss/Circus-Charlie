CXX := g++
CXXFLAGS := -std=c++17 -Iinclude -O2
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lbox2d

SRCS := $(wildcard src/*.cpp)
TARGET := bin/circus

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
