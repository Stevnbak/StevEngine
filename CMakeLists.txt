cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(StevEngine VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

if(CMAKE_BUILD_TYPE MATCHES "Debug")
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# build StevEngine as static library
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "src/*.hpp")
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC src)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
	target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_DEBUGGING)
endif()

# Windows?
if (WIN32)
	target_compile_definitions(${PROJECT_NAME} PUBLIC WINDOWS)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:${CMAKE_BUILD_TYPE}>:${CMAKE_BUILD_TYPE}>")
	set(MSVC_LIBRARY "MultiThreaded$<$<CONFIG:${CMAKE_BUILD_TYPE}>:${CMAKE_BUILD_TYPE}>")
	set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY ${MSVC_LIBRARY})
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

if(USE_RENDERER_GL OR USE_PHYSICS)
	target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_MODELS)
endif()

option(USE_AUDIO "Use the audio system" ON)
if (USE_AUDIO)
	target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_AUDIO)
	set(build_string "${build_string}, AUDIO")
endif()

message("Building ${build_string}")

# external libraries
set(BUILD_SHARED_LIBS OFF)

## GLAD
if(USE_RENDERER_GL)
	option(BUILD_GLAD "Build GLAD as part of the build process" ON)
	if(BUILD_GLAD)
		add_subdirectory(libs/glad/cmake)
		glad_add_library(glad STATIC QUIET REPRODUCIBLE API gl:core=4.4)
		target_link_libraries(${PROJECT_NAME} PUBLIC glad)
	else()
		option(GLAD_DIR "Directory to find the glad files in" ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad)
		target_include_directories(${PROJECT_NAME} PUBLIC ${GLAD_DIR}/include)
		target_sources(${PROJECT_NAME} PRIVATE ${GLAD_DIR}/src/gl.c)
	endif()
endif()

## Jolt
if(USE_PHYSICS)
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
	if(WIN32)
		set_property(TARGET Jolt PROPERTY MSVC_RUNTIME_LIBRARY ${MSVC_LIBRARY})
	endif()
endif()

set(_sdl_cmake_module_path "${CMAKE_MODULE_PATH}")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

## SDL2
set(SDL_SHARED OFF)
set(SDL_STATIC ON)
set(SDL2_DISABLE_INSTALL OFF)
set(SDL_TEST OFF)
add_subdirectory(libs/SDL2)
if(TARGET SDL2::SDL2main)
	target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2main)
endif()
if(TARGET SDL2::SDL2-static)
	target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2-static)
else()
	message(WARNING "Can't find SDL2-static, using SDL2 shared instead!")
	target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2)
endif()
if (WIN32)
	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2::SDL2>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
		VERBATIM
	)
endif()

## SDL_Mixer
if(USE_AUDIO)
	set(SDL2MIXER_VENDORED ON)
	set(SDL2MIXER_MOD OFF)
	set(SDL2MIXER_MIDI OFF)
	set(SDL2MIXER_OPUS OFF)
	set(SDL2MIXER_WAVPACK OFF)
	add_subdirectory(libs/SDL2_mixer)
	target_include_directories(${PROJECT_NAME} PUBLIC libs/SDL2_mixer/include)
	if (WIN32)
		target_link_libraries(${PROJECT_NAME} PUBLIC SDL2_mixer::SDL2_mixer)
		add_custom_command(
			TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2_mixer::SDL2_mixer>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
			VERBATIM
		)
	else()
		target_link_libraries(${PROJECT_NAME} PUBLIC SDL2_mixer)
	endif()
endif()

## SDL_Image
if(USE_RENDERER_GL)
	add_subdirectory(libs/SDL2_image)

	if (WIN32)
		target_link_libraries(${PROJECT_NAME} PUBLIC SDL2_image::SDL2_image)

		add_custom_command(
			TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2_image::SDL2_image>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
			VERBATIM
		)
	else()
		target_link_libraries(${PROJECT_NAME} PUBLIC SDL2_image)
	endif()
endif()

## Assimp
if(USE_RENDERER_GL OR USE_PHYSICS)
	message("Configurating Assimp...")
	set(ASSIMP_NO_EXPORT ON)
	set(ASSIMP_BUILD_TESTS OFF)
	set(ASSIMP_INSTALL OFF)
	set(ASSIMP_BUILD_ASSIMP_VIEW OFF)
	add_subdirectory(libs/assimp)
	target_link_libraries(${PROJECT_NAME} PRIVATE assimp)
	target_include_directories(${PROJECT_NAME} PUBLIC libs/assimp/include ${CMAKE_CURRENT_BINARY_DIR}/libs/assimp/include)
	if(WIN32)
		set_property(TARGET assimp PROPERTY MSVC_RUNTIME_LIBRARY ${MSVC_LIBRARY})
	endif()
endif()

## YAML
set(YAML_CPP_BUILD_CONTRIB OFF)
set(YAML_CPP_BUILD_TOOLS OFF)
add_subdirectory(libs/yaml-cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC yaml-cpp::yaml-cpp)

## uuidv7
target_include_directories(${PROJECT_NAME} PUBLIC libs/uuidv7-h)

# Debug executable
option(DEBUG_EXECUTABLE "Build the debug programs for testing the engine." OFF)
if(DEBUG_EXECUTABLE)
	set(executables "development" "performance")
	FOREACH (executable IN LISTS executables)
		# debug executables
		file(GLOB_RECURSE ${executable}_src CONFIGURE_DEPENDS "debug/${executable}/*.?pp")
		add_executable(debug_${executable} ${${executable}_src})
		target_link_libraries(debug_${executable} PRIVATE ${PROJECT_NAME})
		# embed resources
		include(${CMAKE_CURRENT_SOURCE_DIR}/libs/CMRC/CMakeRC.cmake)
		file(GLOB_RECURSE ASSETS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/debug/${executable}/assets/*")
		cmrc_add_resource_library(debug_${executable}_assets ${ASSETS})
		target_link_libraries(debug_${executable} PRIVATE debug_${executable}_assets)
		#Windows
		if(WIN32)
			set_property(TARGET debug_${executable}_assets PROPERTY MSVC_RUNTIME_LIBRARY ${MSVC_LIBRARY})
			set_property(TARGET debug_${executable} PROPERTY MSVC_RUNTIME_LIBRARY ${MSVC_LIBRARY})
			set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT debug_${executable})
		endif()
	ENDFOREACH()
endif()

# Copy files
if(NOT CMAKE_BUILD_TYPE MATCHES "Debug")
	set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/out)
	make_directory(${OUTPUT_DIR})
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}>  ${OUTPUT_DIR}/lib${PROJECT_NAME}.a
	)
	make_directory(${OUTPUT_DIR}/include)
	foreach(header IN LISTS HEADERS)
		cmake_path(RELATIVE_PATH header BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/ OUTPUT_VARIABLE header_output)
		message(${header_output})
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy ${header} ${OUTPUT_DIR}/include/${header_output}
		)
	endforeach()
endif()