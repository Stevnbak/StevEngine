# StevEngine
Custom game engine written in C++

# Setup & Build (linux)

Clone and update submodules by running:
```shell
git submodule init
git submodule update
```

Build SDL2, SDL_Mixer, JoltPhysics and Tinyxml2 by running the shell script in Build.

Please note:
- *SDL2 requires sudo privileges to install files in `/usr/local`*
```shell
cd Build
./linux_libraries.sh
```
Finally build StevEngine by running `make build` in the `build`folder.

## GLAD setup
glad is added as a library based on [this configuration](https://glad.dav1d.de/#language=c&specification=gl&api=gl%3D3.3&api=gles1%3Dnone&api=gles2%3Dnone&api=glsc2%3Dnone&profile=core&loader=on).


