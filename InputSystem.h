#pragma once
#include <SDL.h> 

namespace StevEngine::InputSystem {
	bool IsKeyPressed(SDL_Keycode key);
	void ForcePressKey(SDL_Keycode key, bool value);
	void KeyDown(SDL_Keycode key);
	void KeyUp(SDL_Keycode key);
}