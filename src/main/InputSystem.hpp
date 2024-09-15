#pragma once
#ifdef StevEngine_INPUTS
#include "main/EventSystem.hpp"
#include "utilities/Vector2.hpp"

#include <SDL.h>

#include <functional>

namespace StevEngine::InputSystem {
	void Init();
	void Update(double deltaTime);

	//Key inputs
	bool IsKeyPressed(SDL_Keycode key);
	void ForcePressKey(SDL_Keycode key, bool value);
	void KeyDown(SDL_Keycode key);
	void KeyUp(SDL_Keycode key);
	//Mouse inputs
	extern Utilities::Vector2 mousePosition;
	extern Utilities::Vector2 mouseDelta;
	extern double mouseWheelDelta;
	void MouseMotion(double X, double Y, double DeltaX, double DeltaY);
	void MouseWheel(double value);
	void ResetMouseDelta();
	//Input events
	void AddKeyDownEvent(std::function<void(SDL_Keycode)> method);
	void AddKeyUpEvent(std::function<void(SDL_Keycode)> method);
	void AddMouseWheelEvent(std::function<void(double)> method);

	#ifdef StevEngine_SHOW_WINDOW
	//Cursor mode
	enum CursorMode {
		Free,
		Confined,
		Locked
	};
	extern CursorMode cursorMode;
	extern bool cursorVisible;
	#endif

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
