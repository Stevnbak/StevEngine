#pragma once
#include "utilities/ID.hpp"
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace StevEngine {
	class Event {
		public:
			virtual const std::string GetEventType() const = 0;
	};
	template<typename EventType> using EventFunction = std::function<void(const EventType& e)>;

	class EventHandlerBase {
		public:
			virtual void Execute(const Event& e) = 0;
			Utilities::ID GetType() const { return handlerType; }
			virtual ~EventHandlerBase() {}
			const Utilities::ID handlerType;
	};

	template<typename EventType>
	class EventHandler : public EventHandlerBase {
		public:
			explicit EventHandler(const EventFunction<EventType>& handler) : handler(handler) {};
			void Execute(const Event& event) override
			{
				if (event.GetEventType() == EventType::GetStaticEventType()) {
					handler(static_cast<const EventType&>(event));
				}
			}
			~EventHandler() {}
		private:
			EventFunction<EventType> handler;
	};

	class EventManager {
		public:
			template<typename EventType> Utilities::ID Subscribe(EventFunction<EventType> handler) {
				return Subscribe(EventType::GetStaticEventType(), new EventHandler<EventType>(handler));
			}
			void Unsubscribe(const std::string eventId, const Utilities::ID handler);
			void Publish(const Event& event);
		private:
			Utilities::ID Subscribe(const std::string eventId, EventHandlerBase* handler);
			std::unordered_map<std::string, std::vector<EventHandlerBase*>> subscribers;
	};
}
