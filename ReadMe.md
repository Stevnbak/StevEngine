# StevEngine
Custom game engine written in C++

# Setup & Build (linux)

Clone and update submodules by running:
```shell
git submodule init
git submodule update
```

Build SDL2, GLEW and JoltPhysics by running the shell script in Build.

Please note:
- *SDL2 and GLEW requires sudo privileges to install files in `/usr/local`*
- *GLEW requires some build tools and OpenGL libraries to be installed, see more [here](https://github.com/nigels-com/glew/tree/glew-2.2.0?tab=readme-ov-file#linux-and-mac)*
- *GLEW requires python to be installed, set the python installation name in `Build/linux_libraries.sh` to match your installation*
```shell
cd Build
./linux_libraries.sh
```

Finally build StevEngine by running `make build` in the `Build`folder.
