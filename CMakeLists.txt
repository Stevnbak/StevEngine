cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(StevEngine VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# build StevEngine as static library
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "src/*.hpp")
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC src)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
	target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_DEBUGGING)
endif()

# modularity options

set(build_string "StevEngine with BASE")

option(USE_INPUTS "Use the input manager" ON)
if (USE_INPUTS)
	target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_INPUTS)
    set(build_string "${build_string}, INPUTS")
endif()

option(USE_PLAYER_DATA "Use the data manager" ON)
if (USE_PLAYER_DATA)
	target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_PLAYER_DATA)
    set(build_string "${build_string}, PLAYER_DATA")
endif()

option(SHOW_WINDOW "Show the game window" ON)
if (SHOW_WINDOW)
	target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_SHOW_WINDOW)
    set(build_string "${build_string}, WINDOW")
endif()

option(USE_RENDERER_GL "Use the OpenGL renderer" ON)
if (USE_RENDERER_GL AND SHOW_WINDOW)
	target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_RENDERER_GL)
    set(build_string "${build_string}, RENDERER_GL")
endif()


option(USE_PHYSICS "Use the physics engine" ON)
if (USE_PHYSICS)
	target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_PHYSICS)
    set(build_string "${build_string}, PHYSICS")
endif()

option(USE_AUDIO "Use the audio system" ON)
if (USE_AUDIO)
	target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_AUDIO)
    set(build_string "${build_string}, AUDIO")
endif()

message("Building ${build_string}")

# external libraries

set(BUILD_SHARED_LIBS OFF)

if(USE_RENDERER_GL)
    ## GLAD
    target_sources(${PROJECT_NAME} PRIVATE libs/glad/src/glad.c)
    target_include_directories(${PROJECT_NAME} PUBLIC libs/glad/include)
endif()

## glm
target_include_directories(${PROJECT_NAME} PUBLIC libs/glm)

if(USE_PHYSICS)
    ## Jolt
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
endif()

## SDL2
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2-static)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
if(TARGET SDL2::SDL2main)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2-static)

if(USE_AUDIO)
    ## SDL_Mixer
    find_package(SDL2_mixer REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC SDL2_mixer::SDL2_mixer)
endif()

## SDL_Image
#find_package(SDL2_image REQUIRED)
#target_link_libraries(${PROJECT_NAME} PUBLIC SDL2_image)

## Assimp
if(USE_RENDERER_GL)
    message("Configurating Assimp...")
    set(ASSIMP_NO_EXPORT ON)
    set(ASSIMP_BUILD_TESTS OFF)
    set(ASSIMP_INSTALL OFF)
    set(ASSIMP_BUILD_ASSIMP_VIEW OFF)
    add_subdirectory(libs/assimp)
    target_link_libraries(${PROJECT_NAME} PRIVATE assimp)
    target_include_directories(${PROJECT_NAME} PUBLIC libs/assimp/include)
    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/libs/assimp/include)
endif()
## Tinyxml2
target_sources(${PROJECT_NAME} PUBLIC libs/TinyXML2/tinyxml2.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC libs/TinyXML2)

## uuidv7
target_include_directories(${PROJECT_NAME} PUBLIC libs/uuidv7-h)

# build debug executable
option(DEBUG_EXECUTABLE "Build the debug program for testing the engine." OFF)
if(DEBUG_EXECUTABLE)
    add_executable(debug debug/main.cpp)
    target_link_libraries(debug PRIVATE ${PROJECT_NAME})
    # embed resources
    set(debug_asset_dir ${CMAKE_BINARY_DIR}/assets)
    if (NOT EXISTS ${debug_asset_dir})
        file(MAKE_DIRECTORY ${debug_asset_dir})
    endif ()

    file(GLOB_RECURSE ASSETS CONFIGURE_DEPENDS "assets/*")
    set(output_c "#include \"assets.h\"\n")
    set(output_h "")
    foreach(asset IN ITEMS ${ASSETS})
        file(RELATIVE_PATH asset_name ${CMAKE_CURRENT_SOURCE_DIR}/assets ${asset})
        string(REPLACE "." "_" asset_name ${asset_name})
        file(READ ${asset} content HEX)
        string(LENGTH ${content} len)
        string(APPEND output_c "
const char* ${asset_name}_data = \"${content}\"\;
const unsigned ${asset_name}_size = ${len}/2\;
")
        string(APPEND output_h "
#ifndef ${asset_name}_H
#define ${asset_name}_H
#include \"stdint.h\"
extern const char* ${asset_name}_data\;
extern const unsigned ${asset_name}_size\;
#endif // ${asset_name}_H
")
    endforeach()
    file(WRITE ${debug_asset_dir}/assets.c ${output_c})
    file(WRITE ${debug_asset_dir}/assets.h ${output_h})
    target_sources(debug PUBLIC ${debug_asset_dir}/assets.c)
    target_include_directories(debug PUBLIC ${debug_asset_dir})
endif()
