#pragma once
#include <Visuals/Camera.hpp>
#include <Physics/System.hpp>

namespace StevEngine {
	extern int targetFPS;
	extern double currentFPS;
	extern Camera* ActiveCamera;
	extern SDL_Window* window;
	extern Physics::System* physics;
	int StartEngine(const char * title, bool fullScreen, void (*mainUpdate)(double deltaTime) = {}, void (*mainStart)() = {});
}