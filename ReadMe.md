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
