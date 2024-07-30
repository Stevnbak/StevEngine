# StevEngine

Custom game engine written in C++

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

Although not recommended it can be disabled by setting the cmake build option "USE_RENDERER_GL" to OFF.

## **Physics**

Physics is implemented as components but is using [Jolt Physics](https://github.com/jrouwe/JoltPhysics) as the backbone.

Can be disabled by setting the cmake build option "USE_PHYSICS" to OFF".

## **Audio**

The audio system allows playing audio through the use of [SDL_Mixer](https://github.com/libsdl-org/SDL_mixer).

Can be disabled by setting the cmake build option "USE_AUDIO" to OFF".

# Setup & Build

Clone and update submodules by running:
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

Build StevEngine and all other external libraries by running the following command (replace `<compiler>` with your c++ compiler such as g++ or clang++):
```shell
cd build && CXX=<compiler> cmake .. --build
```
> **&#9432;** Note: This can take a few minutes on the first build, as it will be building dependencies.

## Build options
The engine has build options for using the engine in the way you want to.

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
