CXX       := g++-13
CXX_FLAGS := -std=c++23 -ggdb -fpermissive

BIN     := bin
SRC     := src
INCLUDE := include

LIBRARIES   := `sdl2-config --cflags --libs` -lGLEW -lGLU -lGL
EXECUTABLE  := main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -I$(INCLUDE2) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*