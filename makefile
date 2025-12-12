CXX = g++
CXXFLAGS = -g -Wall `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_image

SRC = game/main.cpp game/menu.cpp game/level1.cpp
OBJ = $(SRC:.cpp=.o)

juego: $(OBJ)
	$(CXX) -o juego $(OBJ) $(LDFLAGS)

clean:
	rm -f game/*.o juego
