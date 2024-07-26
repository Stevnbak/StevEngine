# StevEngine
Custom game engine written in C++

# Setup & Build

Clone and update submodules by running:
```shell
git submodule update --init
```

Install SDL2 and SDL2_Mixer system wide:

- SDL2:
    - Linux: `sudo apt-get install libsdl2-dev`
    - Windows: Download from https://github.com/libsdl-org/SDL/releases
- SDL2_Mixer
    - Linux: `sudo apt-get install libsdl2-mixer-dev`
    - Windows: Download from https://github.com/libsdl-org/SDL_mixer/releases
- SDL2_Image
    - Linux: `sudo apt-get install libsdl2-image-dev` (also see https://trenki2.github.io/blog/2017/07/04/using-sdl2-image-with-cmake/)
    - Windows: Download from https://github.com/libsdl-org/SDL_image/releases

Build StevEngine and all other external libraries by running (replace `<compiler>` with your c++ compiler such as g++ or clang++):
```shell
cd build && CXX=<compiler> cmake .. --build
```


