# StevEngine
StevEngine is a customizable C++ game engine library.

Features includes:

- Scenes and GameObjects
- Rendering
- Physics
- Inputs
- Audio
- Data management
- Loading models, images and other resources
- Networking
- And more...

The full documentation for this project can be accessed at [stevnbak.github.io/StevEngine](https://stevnbak.github.io/StevEngine) [TOC]

# Modules
The engine constists of multiple modules which can be enabled or disabled during the cmake build process.

These modules are:

## Base

The base of the engine consists of the main engine setup, resource manager, logging, scenes, game-objects and component system.

The main engine setup handles the game window, and main engine loop.

The resource manager handles how files can be used for engine related aspects such as loading objects or scenes from files, audio, images etc.

The logging system handles logging both game and engine information to a file and in debug mode also to the console.

A scene handles all its game-objects which in turn handles its components, more than one scene can exist but only one can be active at a time.

Game-objects are the main way to interact with the engine, a game-object can have other game-objects as children as well as components.
Components are how custom behaviour (both game specific stuff and general stuff like rendering and physics) can be added to a game-object.

This is the only module which cannot be disabled.

## Inputs

The input manager handles inputs from the mouse and keyboard through [SDL](https://github.com/libsdl-org/SDL/tree/SDL2) input events.

Can be disabled by setting the cmake build option `USE_INPUTS` to `OFF`.

## Data & Settings

The data and settings managers handles saving player data and settings to a local folder on the user's machine, which will persist across game restarts.

The settings manager handles saving player settings to a local file on the user's machine, which will persist across game restarts.
This is also the system that is used for for logging to a file. It is therefore not recommended to disable this.

Can be disabled by setting the cmake build option `USE_PLAYER_DATA` to `OFF`.

## OpenGL Renderer

The OpenGL renderer is the only renderer in the engine, it handles (once it's complete) rendering primitives, models, textures, lighting etc.

Can be disabled by setting the cmake build option `USE_RENDERER_GL` to `OFF`.

## Audio

The audio system allows playing audio through the use of [SDL_Mixer](https://github.com/libsdl-org/SDL_mixer).

Can be disabled by setting the cmake build option `USE_AUDIO` to `OFF`.

## Physics

Physics is implemented as components, but is using [Jolt Physics](https://github.com/jrouwe/JoltPhysics) as the backbone.

Can be disabled by setting the cmake build option `USE_PHYSICS` to `OFF`.

## Networking

The networking handles connections and sending messages back and forth, but what data should be sent when and how it's handled is not controlled.

By default it's turned off, can be enabled by setting the cmake build option `USE_NETWORKING` to `ON`.

# Setup & Building

This project is tested and built to be working on Linux (Ubuntu & Arch) and Windows (Using Visual Studio).

And uses CMake as the build system.

## Setup project & dependencies

Clone this repository by running:
```shell
git clone --recursive https://github.com/Stevnbak/StevEngine.git
```

Clone and initialize submodules by running:
```shell
git submodule update --init --recursive --remote
```

To add as submodule to an existing git repository do:
```shell
git submodule add --name StevEngine https://github.com/Stevnbak/StevEngine.git <path>
git submodule update --init --recursive --remote
```

Building or installing [glad](https://github.com/Dav1dde/glad):
> [!WARNING]
> Building glad requires a python intepreter installed and added to the path.
>
> Alternatively glad can be generated online through this link: https://gen.glad.sh/#generator=c&api=gl%3D4.5&profile=gl%3Dcore%2Cgles1%3Dcommon and the build options `BUILD_GLAD` can be set to `OFF` and `GLAD_DIR` should be set to the directory of the installed GLAD files.
>
> (*This can be done by adding `-DBUILD_GLAD=OFF -DGLAD_DIR=<glad_path>` to the cmake command*)

## Linux

Build StevEngine and all other external libraries by running CMake with the following commands:
```shell
CXX=<compiler> cmake -S . -B build -Wno-dev -DCMAKE_BUILD_TYPE=<type>
cmake --build build
```
> [!IMPORTANT]
> - Replace  `<compiler>` with your c++ compiler such as g++ or clang++.
> - Replace `<type>` with `Release` or `Debug`.

> [!NOTE]
> The different build options can be set by adding `-DOPTION=VALUE` to the end of the first command.

> [!NOTE]
> This can take a few minutes on the first build, as it will be building dependencies.

## Windows

Create a Visual Studio solution with all dependencies and build process setup by running CMake with the following command:
```shell
cmake -S . -B VS2022 -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=<type>
```

> [!IMPORTANT]
> - Replace `<type>` with `Release` or `Debug`.
> - Replace `Visual Studio 17 2022` with your Visual Studio version.

> [!NOTE]
> The different build options can be set by adding `-DOPTION=VALUE` to the end of the command.

This solution can now be found in the folder `VS2022` and building the project can be done from Visual Studio.

## Build options

The engine has build options for using the engine with the features and in the way you want to use it.

These options are:

- **Debug & Release**

  Debug symbols will only be available when built with the CMAKE_BUILD_TYPE set to "Debug".
  Whether built as Debug or Release will be passed along to dependencies such as Jolt.

- **Debug executables**

  The debug executable can be enabled through setting the build option "DEBUG_EXECUTABLE" to either ON.

  This is used for testing the engine itself by building the debug program found in "./debug" and has no effect (except longer build time) when using the engine for a separate project.

- **Disable any module (except the base)**

  Any of the engine modules can be disabled by setting the build option "USE_[module]" to OFF.

- **Disable the game window**

  The entire game window can be disabled by setting the build option "SHOW_WINDOW" to OFF.

  This will also disable the renderer module even if that is set to ON.

# Usage

To start using StevEngine for your own project, follow these 3 steps:

## 1. Including the library

Using the StevEngine library requires including the built static library in your own C++ project, and that the StevEngine header files are included.

This can be done simply through adding StevEngine as a CMake submodule:
```cmake
add_subdirectory(path/to/StevEngine)
target_link_libraries(target PUBLIC/PRIVATE/INTERFACE StevEngine)
```
Or include the StevEngine library through any other means of adding a static library to your build system.

## 2. Creating engine instance

To start running StevEngine and all it's subsystems, the first step is to create the instance of the StevEngine::Engine class.

This can be any form of allocated memory, as long as it stays alive for the entire program.

The StevEngine::Engine object and all enabled subsystems is created through the function StevEngine::CreateEngine, that takes a couple of project specific arguments (such as title, target FPS and window options).

Example of creating the engine object inside the main function:
```c++
int main(int argc, char** argv) {
	StevEngine::CreateEngine("Game title", {});
	//Rest of the main function...
}
```

## 3. Starting the engine loop

After creating the engine, and any GameObjects used by the project.
The main loop of the engine can be started by simply calling the Start method of the engine object:
```c++
int main(int argc, char** argv) {
	StevEngine::CreateEngine("Game title", {});
	//Rest of the main function...
	return StevEngine::engine->Start();
}
```
> [!NOTE]
> The engine object can always be accessed through `StevEngine::engine`

> [!IMPORTANT]
> This function will run the entire engine, and will only return a status code once the game is closed by the user or the program.
> It should therefore be called at the very end of the main function.