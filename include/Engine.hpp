#pragma once
#include "Camera.hpp"

namespace StevEngine {
	extern int targetFPS;
	extern Camera* ActiveCamera;
	extern SDL_Window* window;
	void StartEngine(const char * title, bool fullScreen, void (*mainUpdate)(double deltaTime) = {}, void (*mainStart)() = {});
}