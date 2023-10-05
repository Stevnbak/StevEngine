#pragma once
#include <SDL2/include/SDL.h>
#include "Utilities.hpp"
#include <functional>

namespace StevEngine::InputSystem {
	//Key inputs
	bool IsKeyPressed(SDL_Keycode key);
	void ForcePressKey(SDL_Keycode key, bool value);
	void KeyDown(SDL_Keycode key);
	void KeyUp(SDL_Keycode key);
	//Mouse inputs
	extern Utilities::Vector2d mousePosition;
	extern Utilities::Vector2d mouseDelta;
	extern double mouseWheelDelta;
	void MouseMotion(double X, double Y);
	void MouseWheel(double value);
	void ResetMouseDelta();
	//Input events
	void AddKeyDownEvent(std::function<void(SDL_Keycode)> method);
	void AddKeyUpEvent(std::function<void(SDL_Keycode)> method);
	void AddMouseWheelEvent(std::function<void(double)> method);
}