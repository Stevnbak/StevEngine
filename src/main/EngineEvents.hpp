#pragma once
#include "EventSystem.hpp"
#include "utilities/ID.hpp"
#include <SDL_events.h>
#include <SDL_keycode.h>

namespace StevEngine {
	//Main events
	class EngineStartEvent : public Event {
		public:
			EngineStartEvent() {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "EngineStartEvent"; }
	};
	class EngineQuitEvent : public Event {
		public:
			EngineQuitEvent() {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "EngineQuitEvent"; }
	};
	class EngineUpdateEvent : public Event {
		public:
			EngineUpdateEvent(double deltaTime) : deltaTime(deltaTime) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "EngineUpdateEvent"; }
			double deltaTime;
	};
	#ifdef StevEngine_SHOW_WINDOW
		class EngineDrawEvent : public Event {
			public:
				EngineDrawEvent() {}
				const std::string GetEventType() const override { return GetStaticEventType(); };
				static const std::string GetStaticEventType() {  return "EngineDrawEvent"; }
		};
	#endif
	class SDLEvent : public Event {
		public:
			SDLEvent(SDL_Event event) : event(event) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "SDLEvent"; }
			SDL_Event event;
	};
	//Window events
	#ifdef StevEngine_SHOW_WINDOW
		class WindowResizeEvent : public Event {
			public:
				WindowResizeEvent(unsigned int width, unsigned int height) : width(width), height(height) {}
				const std::string GetEventType() const override { return GetStaticEventType(); };
				static const std::string GetStaticEventType() {  return "WindowResizeEvent"; }
				unsigned int width, height;
		};
		class WindowMoveEvent : public Event {
			public:
				WindowMoveEvent(int x, int y) : x(x), y(y) {}
				const std::string GetEventType() const override { return GetStaticEventType(); };
				static const std::string GetStaticEventType() {  return "WindowMoveEvent"; }
				int x, y;
		};
		class WindowFullscreenEvent : public Event {
			public:
				WindowFullscreenEvent(bool value) : value(value) {}
				const std::string GetEventType() const override { return GetStaticEventType(); };
				static const std::string GetStaticEventType() {  return "WindowFullscreenEvent"; }
				bool value;
		};
		class WindowVSyncEvent : public Event {
			public:
				WindowVSyncEvent(bool value) : value(value) {}
				const std::string GetEventType() const override { return GetStaticEventType(); };
				static const std::string GetStaticEventType() {  return "WindowVSyncEvent"; }
				bool value;
		};
	#endif
}
