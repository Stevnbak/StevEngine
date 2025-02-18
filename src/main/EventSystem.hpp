#pragma once
#include "utilities/ID.hpp"
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace StevEngine {
	/**
	 * @brief Base class for all engine events
	 */
	class Event {
		public:
			/**
			 * @brief Get type identifier for this event
			 * @return String identifier for event type
			 */
			virtual const std::string GetEventType() const = 0;
	};

	/** @brief Function type for event handlers */
	template<typename EventType> using EventFunction = std::function<void(const EventType& e)>;

	/**
	 * @brief Base class for event handlers
	 */
	class EventHandlerBase {
		public:
			/**
			 * @brief Execute handler with event
			 * @param e Event to handle
			 */
			virtual void Execute(const Event& e) = 0;

			/**
			 * @brief Get handler type ID
			 * @return Unique ID for this handler
			 */
			Utilities::ID GetType() const { return handlerType; }

			virtual ~EventHandlerBase() {}

			const Utilities::ID handlerType;  ///< Unique handler identifier
	};

	/**
	 * @brief Typed event handler implementation
	 */
	template<typename EventType>
	class EventHandler : public EventHandlerBase {
		public:
			/**
			 * @brief Create new event handler
			 * @param handler Function to execute
			 */
			explicit EventHandler(const EventFunction<EventType>& handler) : handler(handler) {};

			void Execute(const Event& event) override
			{
				if (event.GetEventType() == EventType::GetStaticEventType()) {
					handler(static_cast<const EventType&>(event));
				}
			}

			~EventHandler() {}

		private:
			EventFunction<EventType> handler;  ///< Handler function
	};

	/**
	 * @brief Manages event subscriptions and publishing
	 */
	class EventManager {
		public:
			/**
			 * @brief Subscribe to event type
			 * @tparam EventType Type of event to subscribe to
			 * @param handler Function to handle event
			 * @return Unique ID for subscription
			 */
			template<typename EventType> Utilities::ID Subscribe(EventFunction<EventType> handler) {
				return Subscribe(EventType::GetStaticEventType(), new EventHandler<EventType>(handler));
			}

			/**
			 * @brief Unsubscribe from event
			 * @param eventId Event type identifier
			 * @param handler Handler ID to unsubscribe
			 */
			void Unsubscribe(const std::string eventId, const Utilities::ID handler);

			/**
			 * @brief Publish event to subscribers
			 * @param event Event to publish
			 */
			void Publish(const Event& event);

		private:
			/**
			 * @brief Internal subscribe implementation
			 * @param eventId Event type identifier
			 * @param handler Handler to subscribe
			 * @return Unique ID for subscription
			 */
			Utilities::ID Subscribe(const std::string eventId, EventHandlerBase* handler);

			/** @brief Map of event subscribers */
			std::unordered_map<std::string, std::vector<EventHandlerBase*>> subscribers;
	};
}
