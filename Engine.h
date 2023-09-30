#pragma once
#include "Camera.h"

namespace StevEngine {
	extern int targetFPS;
	extern Camera ActiveCamera;
	void StartEngine(const char * title, bool fullScreen);
}