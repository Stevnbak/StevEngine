COMPILER=/usr/bin/g++-13

echo "SDL2 requires access to /usr/local, so it will require being ran with sudo"
cd ../libs
echo "Starting to build..."
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

#Jolt
cd $root/JoltPhysics
cd Build
cmake -S . -B Linux_Debug -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=$COMPILER "${@}" -DGENERATE_DEBUG_SYMBOLS=ON -DDEBUG_RENDERER_IN_DEBUG_AND_RELEASE=OFF -DPROFILER_IN_DEBUG_AND_RELEASE=OFF -DUSE_STD_VECTOR=OFF -DDOUBLE_PRECISION=OFF -DUSE_ASSERTS=OFF -DENABLE_OBJECT_STREAM=OFF
cd Linux_Debug && make -j 8 && ./UnitTests

#TinyXML2
cd $root/TinyXML2
make

# Return to ./build
cd $root/../build