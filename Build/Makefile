# Compiler options
CXX       := /usr/bin/g++-13
CXX_FLAGS := -std=c++23 -ggdb
# Platform
TARGET := Linux
# Folder paths
ROOT	:= ../
BIN     := bin
SRC     := Engine
# Libraries
LIBRARIES   := `sdl2-config --cflags --libs` -lGLEW -lGLU -lGL -L${ROOT}Libraries/JoltPhysics/Build/$(TARGET)_Debug -lJolt -I${ROOT}Libraries/JoltPhysics
# Executable
EXECUTABLE  := debug

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)


$(BIN)/$(EXECUTABLE): $(shell find ${ROOT}$(SRC) -name '*.cpp')
	$(CXX) $(CXX_FLAGS) -I${ROOT}$(SRC) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*