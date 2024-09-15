#pragma once
#ifdef StevEngine_INPUTS
#include "main/EventSystem.hpp"
#include "main/EngineEvents.hpp"
#include "utilities/Vector2.hpp"

#include <SDL.h>

#include <functional>

namespace StevEngine {
	enum CursorMode {
		Free,
		Confined,
		Locked
	};

	class InputManager {
		public:
			void Init();
			//Input events
			EventManager* GetEvents() { return &events; };
			//Key functions
			bool IsKeyPressed(SDL_Keycode key);
			void ForcePressKey(SDL_Keycode key, bool value);
			bool IsMouseButtonPressed(Uint8 button);
			Utilities::Vector2 GetMousePosition() { return mousePosition; }
			Utilities::Vector2 GetMouseDelta() { return mouseDelta; }
			//Cursor
			#ifdef StevEngine_SHOW_WINDOW
			CursorMode cursorMode = Free;
			bool cursorVisible = true;
			#endif
		private:
			EventManager events;
			std::unordered_map<SDL_Keycode, bool> inputMap;
			std::unordered_map<Uint8, bool> mouseInputMap;
			Utilities::Vector2 mousePosition;
			Utilities::Vector2 mouseDelta;
			double mouseWheelDelta;
			void HandleSDLEvent(const SDLEvent event);
			void Update(double deltaTime);
			void ResetMouseDelta();
	};
	extern InputManager inputManager;


	//Input events
	#ifdef StevEngine_INPUTS
	class InputKeyDownEvent : public Event {
		public:
			InputKeyDownEvent(SDL_Keycode key) : key(key) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "InputKeyDownEvent"; }
			SDL_Keycode key;
	};
	class InputKeyUpEvent : public Event {
		public:
			InputKeyUpEvent(SDL_Keycode key) : key(key) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "InputKeyUpEvent"; }
			SDL_Keycode key;
	};
	class InputMouseMoveEvent : public Event {
		public:
			InputMouseMoveEvent(int x, int y, int deltaX, int deltaY) : x(x), y(y), deltaX(deltaX), deltaY(deltaY) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "InputMouseMoveEvent"; }
			int x, y, deltaX, deltaY;
	};
	class InputMouseWheelEvent : public Event {
		public:
			InputMouseWheelEvent(float value) : value(value) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "InputMouseWheelEvent"; }
			float value;
	};
	class InputMouseButtonDownEvent : public Event {
		public:
			InputMouseButtonDownEvent(Uint8 button) : button(button) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "InputMouseButtonDownEvent"; }
			Uint8 button;
	};
	class InputMouseButtonUpEvent : public Event {
		public:
			InputMouseButtonUpEvent(Uint8 button) : button(button) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "InputMouseButtonUpEvent"; }
			Uint8 button;
	};
	#endif
}
#endif
