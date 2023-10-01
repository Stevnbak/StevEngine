#include <iostream>
#include "InputSystem.h"
#include <SDL.h> 
#include <map>

namespace StevEngine::InputSystem {
	std::map<SDL_Keycode, bool> inputMap;

	bool IsKeyPressed(SDL_Keycode key) {
		bool pressed = false;
		if (inputMap.contains(key)) {
			pressed = inputMap[key];
		}
		std::cout << "Key (" << SDL_GetKeyName(key) << ") has pressed value of " << pressed << std::endl;
		return pressed;
	}

	void ForcePressKey(SDL_Keycode key, bool value) {
		std::cout << "Chganged pressed status of key (" << SDL_GetKeyName(key) << ") to " << value << std::endl;
		inputMap[key] = value;
	}

	void KeyDown(SDL_Keycode key) {
		std::cout << "Recieved input event for key down: " << SDL_GetKeyName(key) << std::endl;
		inputMap[key] = true;
	}

	void KeyUp(SDL_Keycode key) {
		std::cout << "Recieved input event for key up: " << SDL_GetKeyName(key) << std::endl;
		inputMap[key] = false;
	}
}

