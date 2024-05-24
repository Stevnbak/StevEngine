# StevEngine
Custom game engine written in C++

# Setup (linux)

Requires SDL2 and GLEW installed as system libraries
```sh
sudo apt-get install libsdl2-dev
```
```sh
sudo apt-get install libglew-dev
```

Requires JoltPhysics to be built first, see [JoltPhysics Documentation](https://github.com/jrouwe/JoltPhysics/blob/master/Build/README.md#building).
Use these options in thee .sh or .bat (for example in JoltPhysics/Build/cmake_linux_clang_gcc.sh) file: `-DGENERATE_DEBUG_SYMBOLS=ON -DDEBUG_RENDERER_IN_DEBUG_AND_RELEASE=OFF -DPROFILER_IN_DEBUG_AND_RELEASE=OFF -DENABLE_OBJECT_STREAM=OFF`
Full example command from project root: `cd Libraries/JoltPhysics/Build && ./cmake_linux_clang_gcc.sh Debug /usr/bin/g++-13 && cd Linux_Debug && make -j 8 && ./UnitTests`

And update MakeFile to match current build target.

## Useful links
* https://www.gamedesigning.org/learn/make-a-game-engine/
* https://www.gamedeveloper.com/blogs/how-to-make-your-own-c-game-engine
* https://medium.com/geekculture/how-to-make-your-own-game-engine-and-why-ddf0acbc5f3
