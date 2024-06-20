#include "InputSystem.hpp"
#include <core/Log.hpp>
#include <core/Engine.hpp>
#include <core/scenes/GameObject.hpp>

#include <SDL2/SDL.h> 
#include <GL/glew.h>

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

		///Log::Normal(std::format("Key ({}) has pressed value of {}", SDL_GetKeyName(key), pressed), true);
		return pressed;
	}
	void ForcePressKey(SDL_Keycode key, bool value) {
		///Log::Normal(std::format("Changed pressed status of key ({}) to {}", SDL_GetKeyName(key), value), true);
		inputMap[key] = value;
	}
	void KeyDown(SDL_Keycode key) {
		///Log::Normal(std::format("Recieved input event ({}) for key down: {}", key, SDL_GetKeyName(key)), true);
		inputMap[key] = true;
		//Call input event functions
		for (std::function<void(SDL_Keycode)> method : KeyDownEvents) {
			method(key);
		}
	}
	void KeyUp(SDL_Keycode key) {
		///Log::Normal(std::format("Recieved input event ({}) for key up: {}", key, SDL_GetKeyName(key)), true);
		inputMap[key] = false;
		//Call input event functions
		for (std::function<void(SDL_Keycode)> method : KeyUpEvents) {
			method(key);
		}
	}
	//Mouse inputs:
	Utilities::Vector2 mousePosition = Utilities::Vector2();
	Utilities::Vector2 mouseDelta = Utilities::Vector2();
	double mouseWheelDelta = 0;
	void MouseMotion(double X, double Y, double DeltaX, double DeltaY) {
		mousePosition = Utilities::Vector2(X, Y);
		///Log::Normal(std::format("Recieved mouse movement. X: {} Y: {}", mousePosition.X, mousePosition.Y), true);
		//Calculate delta movements
		mouseDelta = Utilities::Vector2(DeltaX, DeltaY);
		///Log::Normal(std::format("Mouse delta X: {} Mouse delta Y: {}", mouseDelta.X, mouseDelta.Y), true);
	}
	void MouseWheel(double value) {
		mouseWheelDelta = value;
		///Log::Normal(std::format("Mouse wheel: {}", mouseWheelDelta), true);
		//Call input event functions
		for (std::function<void(double)> method : MouseWheelEvents) {
			method(value);
		}
	}
	void ResetMouseDelta() {
		mouseDelta = Utilities::Vector2();
		///Log::Normal(std::format("Mouse delta X: {} Mouse delta Y: {}", mouseDelta.X, mouseDelta.Y), true);
	}

	//Cursor mode
	CursorMode cursorMode = CursorMode::Free;
	bool cursorVisible = true;

	//Update
	void Update(double deltaTime) {
		if (cursorMode == CursorMode::Locked) {
			SDL_SetRelativeMouseMode(SDL_TRUE);
			cursorVisible = false;
		}
		else {
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}
		SDL_SetWindowGrab(Engine::Instance->window, cursorMode == CursorMode::Confined ? SDL_TRUE : SDL_FALSE);
		SDL_ShowCursor(cursorVisible ? SDL_ENABLE : SDL_DISABLE);
	}
}

