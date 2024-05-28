# Compiler options
CXX       := /usr/bin/g++-13
CXX_FLAGS := -std=c++23 -ggdb -fsanitize=address
# Folder paths
ROOT	:= ../
BIN     := bin
OBJ 	:= obj
LIB 	:= lib
SRC     := Engine
# Libraries
LIBS   := `sdl2-config --cflags --libs` -lGLEW -lGLU -lGL -L${ROOT}Libraries/JoltPhysics/Build/Linux_Debug -lJolt
LIB_INCLUDE := -I${ROOT}Libraries/JoltPhysics
# Executable
DEBUG_EXECUTABLE  := debug
LIB_NAME := StevEngine



# Calculated variables
SRCS := $(wildcard ${ROOT}$(SRC)/*.cpp) $(wildcard ${ROOT}$(SRC)/*/*.cpp)
OBJS := $(SRCS:${ROOT}$(SRC)/%.cpp=$(OBJ)/%.o)
dir_guard=@mkdir -p $(@D)

# Debug command
debug: $(BIN)/$(DEBUG_EXECUTABLE)

# Build command
build: $(LIB)/$(LIB_NAME).a

# Link to debug executable
$(BIN)/$(DEBUG_EXECUTABLE): $(OBJS)
	$(dir_guard)
	$(CXX) $(CXX_FLAGS) -o $@ $^ -I${ROOT}$(SRC) $(LIBS) $(LIB_INCLUDE)

# Link to library archive
$(LIB)/$(LIB_NAME).a: $(OBJS)
	$(dir_guard)
	ar -r $@ $^

# Compile objects
$(OBJ)/%.o: ${ROOT}$(SRC)/%.cpp
	$(dir_guard)
	$(CXX) $(CXX_FLAGS) -c $< -o $@ -I${ROOT}$(SRC) $(LIBS) $(LIB_INCLUDE)

# Clean command
clean:
	-rm -rf $(BIN)
	-rm -rf $(LIB)
	-rm -rf $(OBJ)