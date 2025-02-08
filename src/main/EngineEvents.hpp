#pragma once
#include "EventSystem.hpp"
#include "utilities/ID.hpp"
#include <SDL_events.h>
#include <SDL_keycode.h>

namespace StevEngine {
	/**
	 * @brief Event triggered when engine starts
	 */
	class EngineStartEvent : public Event {
		public:
			EngineStartEvent() {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "EngineStartEvent"; }
	};

	/**
	 * @brief Event triggered when engine is shutting down
	 */
	class EngineQuitEvent : public Event {
		public:
			EngineQuitEvent() {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "EngineQuitEvent"; }
	};

	/**
	 * @brief Event triggered before frame update
	 */
	class PreUpdateEvent : public Event {
		public:
			PreUpdateEvent() {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "PreUpdateEvent"; }
	};

	/**
	 * @brief Event triggered on frame update
	 */
	class UpdateEvent : public Event {
		public:
			/**
			 * @brief Create update event
			 * @param deltaTime Time since last update
			 */
			UpdateEvent(double deltaTime) : deltaTime(deltaTime) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "UpdateEvent"; }
			double deltaTime;  ///< Time since last update in seconds
	};

	#ifdef StevEngine_SHOW_WINDOW
	/**
	 * @brief Event triggered when frame should be drawn
	 */
	class EngineDrawEvent : public Event {
		public:
			EngineDrawEvent() {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "EngineDrawEvent"; }
	};
	#endif

	/**
	 * @brief Event containing raw SDL event
	 */
	class SDLEvent : public Event {
		public:
			/**
			 * @brief Create SDL event wrapper
			 * @param event Raw SDL_Event
			 */
			SDLEvent(SDL_Event event) : event(event) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "SDLEvent"; }
			SDL_Event event;  ///< Raw SDL event data
	};

	//Window events
	#ifdef StevEngine_SHOW_WINDOW
	/**
	 * @brief Event triggered when window is resized
	 */
	class WindowResizeEvent : public Event {
		public:
			/**
			 * @brief Create window resize event
			 * @param width New width
			 * @param height New height
			 */
			WindowResizeEvent(uint32_t width, uint32_t height) : width(width), height(height) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "WindowResizeEvent"; }
			uint32_t width, height;  ///< New window dimensions
	};

	/**
	 * @brief Event triggered when window is moved
	 */
	class WindowMoveEvent : public Event {
		public:
			/**
			 * @brief Create window move event
			 * @param x New X position
			 * @param y New Y position
			 */
			WindowMoveEvent(int x, int y) : x(x), y(y) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "WindowMoveEvent"; }
			int x, y;  ///< New window position
	};

	/**
	 * @brief Event triggered when fullscreen mode changes
	 */
	class WindowFullscreenEvent : public Event {
		public:
			/**
			 * @brief Create fullscreen change event
			 * @param value New fullscreen state
			 */
			WindowFullscreenEvent(bool value) : value(value) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "WindowFullscreenEvent"; }
			bool value;  ///< Whether fullscreen is enabled
	};

	/**
	 * @brief Event triggered when vsync setting changes
	 */
	class WindowVSyncEvent : public Event {
		public:
			/**
			 * @brief Create vsync change event
			 * @param value New vsync state
			 */
			WindowVSyncEvent(bool value) : value(value) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "WindowVSyncEvent"; }
			bool value;  ///< Whether vsync is enabled
	};
	#endif
}
