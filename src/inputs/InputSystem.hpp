#pragma once
#ifdef StevEngine_INPUTS
#include "main/EventSystem.hpp"
#include "main/EngineEvents.hpp"
#include "utilities/Vector2.hpp"

#include <SDL.h>

namespace StevEngine {
	/**
	 * @brief Defines cursor behavior modes
	 */
	enum CursorMode {
		Free,	   ///< Cursor moves freely
		Confined,   ///< Cursor confined to window
		Locked	  ///< Cursor hidden and locked to window center
	};

	/**
	 * @brief Handles input processing and management
	 *
	 * Processes keyboard, mouse and cursor input events. Maintains input state
	 * and provides methods to query input status.
	 */
	class InputManager {
		public:
			/**
			 * @brief Initialize input system
			 */
			void Init();

			/**
			 * @brief Get input events manager
			 * @return Pointer to event manager
			 */
			EventManager* GetEvents() { return &events; };

			/**
			 * @brief Check if key is currently pressed
			 * @param key SDL keycode to check
			 * @return true if key is pressed, false otherwise
			 */
			bool IsKeyPressed(SDL_Keycode key) const;

			/**
			 * @brief Force key press state
			 * @param key SDL keycode to set
			 * @param value Pressed state to set
			 */
			void ForcePressKey(SDL_Keycode key, bool value);

			/**
			 * @brief Check if mouse button is currently pressed
			 * @param button Mouse button ID to check
			 * @return true if button is pressed, false otherwise
			 */
			bool IsMouseButtonPressed(Uint8 button) const;

			/**
			 * @brief Get current mouse position
			 * @return Vector2 containing mouse coordinates
			 */
			Utilities::Vector2 GetMousePosition() const { return mousePosition; }

			/**
			 * @brief Get mouse movement since last frame
			 * @return Vector2 containing mouse movement delta
			 */
			Utilities::Vector2 GetMouseDelta() const { return mouseDelta; }

			#ifdef StevEngine_SHOW_WINDOW
			CursorMode cursorMode = Free;  ///< Current cursor behavior mode
			bool cursorVisible = true;	 ///< Whether cursor is visible
			#endif

		private:
			EventManager events;  ///< Input event manager
			std::unordered_map<SDL_Keycode, bool> inputMap;  ///< Key press state map
			std::unordered_map<Uint8, bool> mouseInputMap;   ///< Mouse button state map
			Utilities::Vector2 mousePosition;  ///< Current mouse position
			Utilities::Vector2 mouseDelta;	 ///< Mouse movement delta
			double mouseWheelDelta;		   ///< Mouse wheel movement

			void HandleSDLEvent(const SDLEvent event);
			void Update(double deltaTime);
			void ResetMouseDelta();
	};

	/** Global input manager instance */
	extern InputManager inputManager;


	//Input events
	/**
	 * @brief Event triggered when a keyboard key is pressed
	 */
	class InputKeyDownEvent : public Event {
		public:
			/**
			 * @brief Create key down event
			 * @param key SDL keycode of pressed key
			 */
			InputKeyDownEvent(SDL_Keycode key) : key(key) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "InputKeyDownEvent"; }
			SDL_Keycode key;  ///< Keycode of pressed key
	};

	/**
	 * @brief Event triggered when a keyboard key is released
	 */
	class InputKeyUpEvent : public Event {
		public:
			/**
			 * @brief Create key up event
			 * @param key SDL keycode of released key
			 */
			InputKeyUpEvent(SDL_Keycode key) : key(key) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "InputKeyUpEvent"; }
			SDL_Keycode key;  ///< Keycode of released key
	};

	/**
	 * @brief Event triggered when mouse is moved
	 */
	class InputMouseMoveEvent : public Event {
		public:
			/**
			 * @brief Create mouse move event
			 * @param x New X position
			 * @param y New Y position
			 * @param deltaX X movement since last position
			 * @param deltaY Y movement since last position
			 */
			InputMouseMoveEvent(int x, int y, int deltaX, int deltaY) : x(x), y(y), deltaX(deltaX), deltaY(deltaY) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "InputMouseMoveEvent"; }
			int x, y;		///< Current mouse position
			int deltaX, deltaY;  ///< Mouse movement delta
	};

	/**
	 * @brief Event triggered when mouse wheel is scrolled
	 */
	class InputMouseWheelEvent : public Event {
		public:
			/**
			 * @brief Create mouse wheel event
			 * @param value Amount and direction of scroll
			 */
			InputMouseWheelEvent(float value) : value(value) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "InputMouseWheelEvent"; }
			float value;  ///< Scroll amount and direction
	};

	/**
	 * @brief Event triggered when mouse button is pressed
	 */
	class InputMouseButtonDownEvent : public Event {
		public:
			/**
			 * @brief Create mouse button down event
			 * @param button ID of pressed button
			 */
			InputMouseButtonDownEvent(Uint8 button) : button(button) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "InputMouseButtonDownEvent"; }
			Uint8 button;  ///< ID of pressed button
	};

	/**
	 * @brief Event triggered when mouse button is released
	 */
	class InputMouseButtonUpEvent : public Event {
		public:
			/**
			 * @brief Create mouse button up event
			 * @param button ID of released button
			 */
			InputMouseButtonUpEvent(Uint8 button) : button(button) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "InputMouseButtonUpEvent"; }
			Uint8 button;  ///< ID of released button
	};
}
#endif
