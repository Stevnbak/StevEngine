#pragma once
#include "EventSystem.hpp"
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
	class PreUpdateEvent : public Event {
		public:
			PreUpdateEvent() {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "PreUpdateEvent"; }
	};
	class UpdateEvent : public Event {
		public:
			UpdateEvent(double deltaTime) : deltaTime(deltaTime) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "UpdateEvent"; }
			double deltaTime;
	};
	#ifdef StevEngine_SHOW_WINDOW
		class EngineDrawEvent : public Event {
			public:
				EngineDrawEvent() {}
				const std::string GetEventType() const override { return GetStaticEventType(); };
				static const std::string GetStaticEventType() {  return "EngineDrawEvent"; }
		};
		class EnginePreDrawEvent : public Event {
			public:
				EnginePreDrawEvent() {}
				const std::string GetEventType() const override { return GetStaticEventType(); };
				static const std::string GetStaticEventType() {  return "EnginePreDrawEvent"; }
		};
		class EnginePostDrawEvent : public Event {
			public:
				EnginePostDrawEvent() {}
				const std::string GetEventType() const override { return GetStaticEventType(); };
				static const std::string GetStaticEventType() {  return "EnginePostDrawEvent"; }
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
				WindowResizeEvent(uint32_t width, uint32_t height) : width(width), height(height) {}
				const std::string GetEventType() const override { return GetStaticEventType(); };
				static const std::string GetStaticEventType() {  return "WindowResizeEvent"; }
				uint32_t width, height;
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
