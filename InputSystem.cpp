#include "Log.hpp"
#include <format>
#include "InputSystem.hpp"
#include "GameObject.hpp"
#include <SDL2/include/SDL.h> 
#include <map>
#include <vector>

namespace StevEngine::InputSystem {
	//Input events
	std::vector<std::function<void(SDL_Keycode)>> KeyDownEvents;
	std::vector<std::function<void(SDL_Keycode)>> KeyUpEvents;
	std::vector<std::function<void(double)>> MouseWheelEvents;
	void AddKeyDownEvent(std::function<void(SDL_Keycode)> method) {
		KeyDownEvents.push_back(method);
	}
	void AddKeyUpEvent(std::function<void(SDL_Keycode)> method) {
		KeyUpEvents.push_back(method);
	}
	void AddMouseWheelEvent(std::function<void(double)> method) {
		MouseWheelEvents.push_back(method);
	}

	//Key Inputs:
	std::map<SDL_Keycode, bool> inputMap;
	bool IsKeyPressed(SDL_Keycode key) {
		bool pressed = false;
		if (inputMap.contains(key)) {
			pressed = inputMap[key];
		}

		Log::Normal(std::format("Key ({}) has pressed value of {}", SDL_GetKeyName(key), pressed), true);
		return pressed;
	}
	void ForcePressKey(SDL_Keycode key, bool value) {
		Log::Normal(std::format("Changed pressed status of key ({}) to {}", SDL_GetKeyName(key), value), true);
		inputMap[key] = value;
	}
	void KeyDown(SDL_Keycode key) {
		Log::Normal(std::format("Recieved input event ({}) for key down: {}", key, SDL_GetKeyName(key)), true);
		inputMap[key] = true;
		//Call input event functions
		for (std::function<void(SDL_Keycode)> method : KeyDownEvents) {
			method(key);
		}
	}
	void KeyUp(SDL_Keycode key) {
		Log::Normal(std::format("Recieved input event ({}) for key up: {}", key, SDL_GetKeyName(key)), true);
		inputMap[key] = false;
		//Call input event functions
		for (std::function<void(SDL_Keycode)> method : KeyUpEvents) {
			method(key);
		}
	}
	//Mouse inputs:
	Utilities::Vector2d mousePosition = Utilities::Vector2d();
	Utilities::Vector2d mouseDelta = Utilities::Vector2d();
	Utilities::Vector2d previousMousePosition = Utilities::Vector2d();
	double mouseWheelDelta = 0;
	void MouseMotion(double X, double Y) {
		mousePosition = Utilities::Vector2d(X, Y);
		Log::Normal(std::format("Recieved mouse movement. X: {} Y: {}", mousePosition.X, mousePosition.Y), true);
		//Calculate delta movements
		mouseDelta = mousePosition - previousMousePosition;
		previousMousePosition = mousePosition;
		Log::Normal(std::format("Mouse delta X: {} Mouse delta Y: {}", mouseDelta.X, mouseDelta.Y), true);
	}
	void MouseWheel(double value) {
		mouseWheelDelta = value;
		Log::Normal(std::format("Mouse wheel: {}", mouseWheelDelta), true);
		//Call input event functions
		for (std::function<void(double)> method : MouseWheelEvents) {
			method(value);
		}
	}
	void ResetMouseDelta() {
		mouseDelta = Utilities::Vector2d();
		///Log::Normal(std::format("Mouse delta X: {} Mouse delta Y: {}", mouseDelta.X, mouseDelta.Y), true);
	}
}

