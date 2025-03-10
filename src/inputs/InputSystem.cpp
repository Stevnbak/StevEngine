#ifdef StevEngine_INPUTS
#include "inputs/InputSystem.hpp"
#include "main/Engine.hpp"
#include "main/EngineEvents.hpp"

#include <SDL.h>

namespace StevEngine {
	InputManager inputManager = InputManager();
	void InputManager::HandleSDLEvent(const SDLEvent event) {
		SDL_Event ev = event.event;
		switch(ev.type) {
			//Input system
			case SDL_KEYDOWN:
				inputMap[ev.key.keysym.sym] = true;
				events.Publish(InputKeyDownEvent(ev.key.keysym.sym));
				break;
			case SDL_KEYUP:
				inputMap[ev.key.keysym.sym] = false;
				events.Publish(InputKeyUpEvent(ev.key.keysym.sym));
				break;
			case SDL_MOUSEMOTION:
				mousePosition = Utilities::Vector2(ev.motion.x, ev.motion.y);
				///Log::Debug(std::format("Recieved mouse movement. X: {} Y: {}", mousePosition.X, mousePosition.Y), true);
				mouseDelta = Utilities::Vector2(ev.motion.xrel, ev.motion.yrel);
				///Log::Debug(std::format("Mouse delta X: {} Mouse delta Y: {}", mouseDelta.X, mouseDelta.Y), true);
				events.Publish(InputMouseMoveEvent(mousePosition.X, mousePosition.Y, mouseDelta.X, mouseDelta.Y));
				break;
			case SDL_MOUSEWHEEL:
				mouseWheelDelta = ev.wheel.preciseY;
				events.Publish(InputMouseWheelEvent(mouseWheelDelta));
				break;
			case SDL_MOUSEBUTTONDOWN:
				mouseInputMap[ev.button.button] = true;
				events.Publish(InputMouseButtonDownEvent(ev.button.button));
				break;
			case SDL_MOUSEBUTTONUP:
				mouseInputMap[ev.button.button] = false;
				events.Publish(InputMouseButtonUpEvent(ev.button.button));
				break;
		}
	}
	void InputManager::Init() {
		engine->GetEvents()->Subscribe<SDLEvent>([this] (SDLEvent e) { this->HandleSDLEvent(e); });
		engine->GetEvents()->Subscribe<UpdateEvent>([this] (UpdateEvent e) { this->Update(e.deltaTime); });
		engine->GetEvents()->Subscribe<PreUpdateEvent>([this] (PreUpdateEvent) { this->ResetMouseDelta(); });
	}
	//Key Inputs:
	bool InputManager::IsKeyPressed(SDL_Keycode key) const {
		bool pressed = false;
		if (inputMap.contains(key)) {
			pressed = inputMap.at(key);
		}
		///Log::Debug(std::format("Key ({}) has pressed value of {}", SDL_GetKeyName(key), pressed), true);
		return pressed;
	}
	void InputManager::ForcePressKey(SDL_Keycode key, bool value) {
		///Log::Debug(std::format("Changed pressed status of key ({}) to {}", SDL_GetKeyName(key), value), true);
		inputMap[key] = value;
		if(value) events.Publish(InputKeyDownEvent(key));
		else events.Publish(InputKeyUpEvent(key));
	}
	bool InputManager::IsMouseButtonPressed(Uint8 button) const {
		bool pressed = false;
		if (mouseInputMap.contains(button)) {
			pressed = inputMap.at(button);
		}
		///Log::Debug(std::format("Mouse button ({}) has pressed value of {}", button, pressed), true);
		return pressed;
	}
	void InputManager::ResetMouseDelta() {
		mouseDelta = Utilities::Vector2();
		///Log::Debug(std::format("Mouse delta X: {} Mouse delta Y: {}", mouseDelta.X, mouseDelta.Y), true);
	}
	//Update
	void InputManager::Update(double deltaTime) {
		#ifdef StevEngine_SHOW_WINDOW
		if (cursorMode == CursorMode::Locked) {
			SDL_SetRelativeMouseMode(SDL_TRUE);
			cursorVisible = false;
		}
		else {
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}
		SDL_SetWindowGrab(engine->window, cursorMode == CursorMode::Confined ? SDL_TRUE : SDL_FALSE);
		SDL_ShowCursor(cursorVisible ? SDL_ENABLE : SDL_DISABLE);
		#endif
	}
}
#endif
