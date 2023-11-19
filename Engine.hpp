#pragma once
#include "Camera.hpp"

namespace StevEngine {
	extern int targetFPS;
	extern Camera* ActiveCamera;
	void StartEngine(const char * title, bool fullScreen, void (*mainUpdate)(double deltaTime) = {}, void (*mainStart)() = {});
}