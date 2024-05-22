#pragma once
#include "Camera.hpp"
#include "Physics.hpp"

namespace StevEngine {
	extern int targetFPS;
	extern Camera* ActiveCamera;
	extern SDL_Window* window;
	extern Physics::System* physics;
	int StartEngine(const char * title, bool fullScreen, void (*mainUpdate)(double deltaTime) = {}, void (*mainStart)() = {});
}