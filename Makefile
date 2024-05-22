# Compiler options
CXX       := /usr/bin/g++-13
CXX_FLAGS := -std=c++23 -ggdb
# Platform
TARGET := Linux
# Folder paths
BIN     := bin
SRC     := src
INCLUDE := include
# Libraries
LIBRARIES   := `sdl2-config --cflags --libs` -lGLEW -lGLU -lGL -LJoltPhysics/Build/$(TARGET)_Debug -lJolt -IJoltPhysics
# Executable
EXECUTABLE  := main

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)


$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*