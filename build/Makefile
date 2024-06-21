# Compiler options
CXX       			:= /usr/bin/g++-13
CXX_FLAGS 			:= -std=c++23
DEBUG_FLAGS			:= -ggdb -fsanitize=address -g
# Folder paths
ROOT				:= ../
LIB 				:= lib
SRC     			:= src
DEBUG_RSC			:= assets
BIN     			:= bin
OBJ 				:= obj

# Libraries
LIBDIR 				:= ${ROOT}libs
LIBS   				:= $(LIBDIR)/JoltPhysics/Build/Linux_Debug/libJolt.a $(LIBDIR)/SDL2/build/build/.libs/libSDL2.a $(LIBDIR)/SDL_Mixer/build/.libs/libSDL2_mixer.a $(LIBDIR)/GLEW/lib/libGLEW.a -lGL $(LIBDIR)/TinyXML2/libtinyxml2.a
LIBS_INCLUDE 		:= -I$(LIBDIR)/JoltPhysics -I$(LIBDIR)/SDL2/include -I$(LIBDIR)/SDL_Mixer/include -I$(LIBDIR)/GLEW/include -I$(LIBDIR)/TinyXML2 -I$(LIBDIR)/uuidv7
# Executable
DEBUG_EXECUTABLE	:= debug
LIB_NAME			:= StevEngine



# Calculated variables
SRCS := $(shell find ${ROOT}$(SRC) -name "*.cpp")
OBJS := $(SRCS:${ROOT}$(SRC)/%.cpp=$(OBJ)/%.o)
ASSETS := $(shell find ${ROOT}$(DEBUG_RSC) -name "*.*")
dir_guard=@mkdir -p $(@D)

# Debug command
debug: $(BIN)/$(DEBUG_EXECUTABLE)

# Build command
build: $(LIB)/$(LIB_NAME).a

# Link to debug executable
$(BIN)/$(DEBUG_EXECUTABLE): $(OBJS) $(OBJ)/assets.o
	$(dir_guard)
	$(CXX) $(CXX_FLAGS) $(DEBUG_FLAGS) -o $@ $^ -I${ROOT}$(SRC) $(LIBS) $(LIBS_INCLUDE)

# Link to library archive
$(LIB)/$(LIB_NAME).a: $(filter-out $(OBJ)/debug.o, $(OBJS))
	$(dir_guard)
	ar -r $@ $^ $(LIBS)

# Compile objects
$(OBJ)/%.o: ${ROOT}$(SRC)/%.cpp
	$(dir_guard)
	$(CXX) $(CXX_FLAGS) -g -c $< -o $@ -I${ROOT}$(SRC) $(LIBS_INCLUDE)

# Embed asset files
$(OBJ)/assets.o: 
	$(dir_guard)
	ld -r -b binary -o $(OBJ)/assets.o $(ASSETS)

# Clean command
clean:
	-rm -rf $(BIN)
	-rm -rf $(LIB)
	-rm -rf $(OBJ)