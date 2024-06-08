COMPILER=/usr/bin/g++-13

echo "SDL2 and GLEW requires access to /usr/local, so these will require being ran with sudo"
cd ../libs
sudo echo "Starting to build..."
root=$(pwd)

#SDL2 https://wiki.libsdl.org/SDL2/Installation#linuxunix
cd $root/SDL2
mkdir build
cd build
../configure
make
sudo make install

#SDL_Mixer
cd $root/SDL_Mixer
./configure
make
sudo make install

#GLEW https://github.com/nigels-com/glew/tree/glew-2.2.0?tab=readme-ov-file#linux-and-mac
cd $root/GLEW
make extensions PYTHON=python3
make CFLAGS.EXTRA="-DGLEW_STATIC"
sudo make install

#Jolt
cd $root/JoltPhysics
cd Build
cmake -S . -B Linux_Debug -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=$COMPILER "${@}" -DGENERATE_DEBUG_SYMBOLS=ON -DDEBUG_RENDERER_IN_DEBUG_AND_RELEASE=OFF -DPROFILER_IN_DEBUG_AND_RELEASE=OFF -DENABLE_OBJECT_STREAM=OFF
cd Linux_Debug && make -j 8 && ./UnitTests

# Return to ./build
cd $root/../build