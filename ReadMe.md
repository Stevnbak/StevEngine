# StevEngine

StevEngine is a customizable game engine C++ library.

Features includes:

- Scenes and GameObjects
- Rendering
- Physics
- Inputs
- Audio
- Data management
- Loading models, images and other resources
- And more...

# Table of Contents

- [Modules](#modules)
  - [Base](#base)
  - [Input Manager](#input-manager)
  - [Data Manager](#data-manager)
  - [OpenGL Renderer](#opengl-renderer)
  - [Physics](#physics)
  - [Audio](#audio)
- [Setup & Building](#setup--building)
  - [Setup project & dependencies](#setup-project--dependencies)
  - [Linux](#linux)
  - [Windows](#windows)
  - [Build options](#build-options)
- [Usage](#usage)

# Modules
The engine constists of multiple modules which can be enabled or disabled during the cmake build process.

These modules are:

## **Base**

The base of the engine consists of the main engine setup, resource manager, logging, scenes, game-objects and component system.

The main engine setup handles the game window, and main engine loop.

The resource manager handles how files can be used for engine related aspects such as loading objects or scenes from files, audio, images etc.

The logging system handles logging both game and engine information to a file and in debug mode also to the console.

A scene handles all its game-objects which in turn handles its components, more than one scene can exist but only one can be active at a time.

Game-objects are the main way to interact with the engine, a game-object can have other game-objects as children as well as components.
Components are how custom behaviour (both game specific stuff and general stuff like rendering and physics) can be added to a game-object.

This is the only module which cannot be disabled.

## **Input Manager**

The input manager handles inputs from the mouse and keyboard through [SDL](https://github.com/libsdl-org/SDL/tree/SDL2) input events.

Can be disabled by setting the cmake build option "USE_INPUTS" to OFF".

## **Data Manager**

The data manager handles saving player data to a local folder on the user's machine, which will persist across game restarts.

This is also the system that is used for saving engine settings across restarts, and for logging to a file.
It is therefore not recommended to disable this.

Can be disabled by setting the cmake build option "USE_PLAYER_DATA" to OFF".

## **OpenGL Renderer**

The OpenGL renderer is the only renderer in the engine, it handles (once it's complete) rendering primitives, models, textures, lighting etc.

Can be disabled by setting the cmake build option "USE_RENDERER_GL" to OFF.

## **Physics**

Physics is implemented as components, but is using [Jolt Physics](https://github.com/jrouwe/JoltPhysics) as the backbone.

Can be disabled by setting the cmake build option "USE_PHYSICS" to OFF".

## **Audio**

The audio system allows playing audio through the use of [SDL_Mixer](https://github.com/libsdl-org/SDL_mixer).

Can be disabled by setting the cmake build option "USE_AUDIO" to OFF".

# Setup & Building

This project is tested and built to be working on Linux (Ubuntu) and Windows (Using Visual Studio).

And uses CMake as the build system

## Setup project & dependencies

Clone this repository by running:
```shell
git clone https://github.com/Stevnbak/StevEngine.git
```

Clone and initialize submodules by running:
```shell
git submodule update --init
```

Install SDL2, SDL2_Image and SDL2_Mixer system wide:

- SDL2:
    - Linux: `sudo apt-get install libsdl2-dev`
    - Windows: Download from https://github.com/libsdl-org/SDL/releases
- SDL2_Mixer
    - Linux: `sudo apt-get install libsdl2-mixer-dev`
    - Windows: Download from https://github.com/libsdl-org/SDL_mixer/releases
- SDL2_Image
    - Linux: `sudo apt-get install libsdl2-image-dev` (also see https://trenki2.github.io/blog/2017/07/04/using-sdl2-image-with-cmake/)
    - Windows: Download from https://github.com/libsdl-org/SDL_image/releases

> [!IMPORTANT]
> When downloading for windows make sure to download the `-devel-X.X.X-VC` option.

## Linux

Build StevEngine and all other external libraries by running CMake with the following command:
```shell
cd build && CXX=<compiler> cmake .. --build -DCMAKE_BUILD_TYPE=<type>
```

> [!IMPORTANT]
> - Replace  `<compiler>` with your c++ compiler such as g++ or clang++.
> - Replace `<type>` with `Release` or `Debug`.

> [!NOTE]
> The different build options can be set by adding `-DOPTION=VALUE` to the end of the command.

> [!NOTE]
> This can take a few minutes on the first build, as it will be building dependencies.

## Windows

Create a Visual Studio solution with all dependencies and build process setup by running CMake with the following command:
```shell
cmake -S . -B VS2022 -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH="<cmake_module_path>" -DCMAKE_BUILD_TYPE=<type>
```

> [!IMPORTANT]
> - Replace `<cmake_module_path>` with the path to the previously downloaded SDL libraries.
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

- **Debug executable**

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

The StevEngine::Engine object is created through it's constructor, that takes a couple of project specific arguments (such as title, target FPS and window options).

Example of creating the engine object inside the main function:
```c++
int main(int argc, char** argv) {
    StevEngine::Engine engine = StevEngine::Engine("Game title", 60);
    //Rest of the main function...
}
```

## 3. Starting the engine loop

After creating the engine, and any GameObjects used by the project.
The main loop of the engine can be started by simply calling the Start method of the engine object:
```c++
int main(int argc, char** argv) {
    StevEngine::Engine engine = StevEngine::Engine("Game title", 60);
    //Rest of the main function...
    return engine.Start();
}
```
> [!NOTE]
> The engine object can also be accessed through `StevEngine::Engine::Instance`

> [!IMPORTANT]
> This function will run the entire engine, and will only return a status code once the game is closed by the user or the program.
> It should therefore be called at the very end of the main function.
