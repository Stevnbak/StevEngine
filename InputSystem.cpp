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

	Utilities::Vector2d mousePosition = Utilities::Vector2d();
	Utilities::Vector2d mouseDelta = Utilities::Vector2d();
	Utilities::Vector2d previousMousePosition = Utilities::Vector2d();
	void MouseMotion() {
		std::cout << "Recieved mouse movement. X: " << mousePosition.X << ". Y: " << mousePosition.Y << std::endl;
		//Calculate delta movements
		mouseDelta = mousePosition - previousMousePosition;
		previousMousePosition = mousePosition;
		std::cout << "Mouse delta X: " << mouseDelta.X << ". Mouse delta Y: " << mouseDelta.Y << std::endl;
	}
	void ResetMouseDelta() {
		mouseDelta = Utilities::Vector2d();
		std::cout << "Mouse delta X: " << mouseDelta.X << ". Mouse delta Y: " << mouseDelta.Y << std::endl;
	}
}

