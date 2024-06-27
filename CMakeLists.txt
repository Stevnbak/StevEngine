cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(StevEngine VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# build StevEngine as static library
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.hpp" "src/*.cpp")
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC src)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)

# external libraries

target_sources(${PROJECT_NAME} PRIVATE libs/glad/src/glad.c)
target_include_directories(${PROJECT_NAME} PUBLIC libs/glad/include)

set(ENABLE_OBJECT_STREAM OFF)
set(USE_ASSERTS OFF)
set(DEBUG_RENDERER_IN_DEBUG_AND_RELEASE OFF)
set(PROFILER_IN_DEBUG_AND_RELEASE OFF)
set(USE_STD_VECTOR ON)
set(GENERATE_DEBUG_SYMBOLS OFF)
set(DOUBLE_PRECISION ON)
add_subdirectory(libs/JoltPhysics/Build)
target_link_libraries(${PROJECT_NAME} PRIVATE Jolt)
target_include_directories(${PROJECT_NAME} PUBLIC libs/JoltPhysics)

# SDL2
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2-static)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
if(TARGET SDL2::SDL2main)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2-static)

# SDL_Mixer
find_package(SDL2_mixer REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2_mixer::SDL2_mixer)

add_subdirectory(libs/TinyXML2)
target_link_libraries(${PROJECT_NAME} PUBLIC tinyxml2)

target_include_directories(${PROJECT_NAME} PUBLIC libs/uuidv7-h)

target_include_directories(${PROJECT_NAME} PUBLIC libs/glm)

# build debug executable
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    add_executable(debug debug/main.cpp)
    target_link_libraries(debug PRIVATE ${PROJECT_NAME})
endif()
