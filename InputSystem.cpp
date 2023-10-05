#include "Log.hpp"
#include <format>
#include "InputSystem.hpp"
#include "GameObject.hpp"
#include <SDL2/include/SDL.h> 
#include <map>

namespace StevEngine::InputSystem {
	std::map<SDL_Keycode, bool> inputMap;

	bool IsKeyPressed(SDL_Keycode key) {
		bool pressed = false;
		if (inputMap.contains(key)) {
			pressed = inputMap[key];
		}

		Log::Normal(std::format("Key ({}) has pressed value of {}", SDL_GetKeyName(key), pressed));
		return pressed;
	}

	void ForcePressKey(SDL_Keycode key, bool value) {
		Log::Normal(std::format("anged pressed status of key ({}) to {}", SDL_GetKeyName(key), value));
		inputMap[key] = value;
	}

	void KeyDown(SDL_Keycode key) {
		Log::Normal(std::format("Recieved input event ({}) for key down: {}", key, SDL_GetKeyName(key)));
		inputMap[key] = true;
	}

	void KeyUp(SDL_Keycode key) {
		Log::Normal(std::format("Recieved input event ({}) for key up: {}", key, SDL_GetKeyName(key)));
		inputMap[key] = false;
	}

	Utilities::Vector2d mousePosition = Utilities::Vector2d();
	Utilities::Vector2d mouseDelta = Utilities::Vector2d();
	Utilities::Vector2d previousMousePosition = Utilities::Vector2d();
	double mouseWheelDelta = 0;
	void MouseMotion(double X, double Y) {
		mousePosition = Utilities::Vector2d(X, Y);
		Log::Normal(std::format("Recieved mouse movement. X: {} Y: {}", mousePosition.X, mousePosition.Y));
		//Calculate delta movements
		mouseDelta = mousePosition - previousMousePosition;
		previousMousePosition = mousePosition;
		Log::Normal(std::format("Mouse delta X: {} Mouse delta Y: {}", mouseDelta.X, mouseDelta.Y));
	}
	void MouseWheel(double value) {
		mouseWheelDelta = value;
		Log::Normal(std::format("Mouse wheel: {}", mouseWheelDelta));
	}
	void ResetMouseDelta() {
		mouseDelta = Utilities::Vector2d();
		///Log::Normal(std::format("Mouse delta X: {} Mouse delta Y: {}", mouseDelta.X, mouseDelta.Y));
	}
}

