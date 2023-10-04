#pragma once
#include <SDL.h> 
#include "Utilities.hpp"

namespace StevEngine::InputSystem {
	bool IsKeyPressed(SDL_Keycode key);
	void ForcePressKey(SDL_Keycode key, bool value);
	void KeyDown(SDL_Keycode key);
	void KeyUp(SDL_Keycode key);
	extern Utilities::Vector2d mousePosition;
	extern Utilities::Vector2d mouseDelta;
	extern double mouseWheelDelta;
	void MouseMotion(double X, double Y);
	void MouseWheel(double value);
	void ResetMouseDelta();
}