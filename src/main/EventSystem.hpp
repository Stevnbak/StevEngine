#pragma once
#include <functional>
#include <memory>
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
			virtual std::string GetType() const = 0;
	};

	template<typename EventType>
	class EventHandler : public EventHandlerBase {
		public:
			explicit EventHandler(const EventFunction<EventType>& handler) : handler(handler), handlerType(handler.target_type().name()) {};
			void Execute(const Event& event) override
			{
				if (event.GetEventType() == EventType::GetStaticEventType()) {
					handler(static_cast<const EventType&>(event));
				}
			}
		private:
			std::string GetType() const override { return handlerType; }
			EventFunction<EventType> handler;
			const std::string handlerType;
	};

	class EventManager {
		public:
			template<typename EventType> std::string Subscribe(EventFunction<EventType> handler) {
				return Subscribe(EventType::GetStaticEventType(), std::make_unique<EventHandler<EventType>>(EventHandler<EventType>(handler)));
			}
			void Unsubscribe(const std::string eventId, const std::string handlerName);
			void Publish(const Event& event);
		private:
			std::string Subscribe(const std::string eventId, std::unique_ptr<EventHandlerBase> handler);
			std::unordered_map<std::string, std::vector<std::unique_ptr<EventHandlerBase>>> subscribers;
	};
}
