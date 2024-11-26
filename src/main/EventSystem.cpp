#include "EventSystem.hpp"

namespace StevEngine {
	Utilities::ID EventManager::Subscribe(const std::string eventId, EventHandlerBase* handler) {
		auto subs = subscribers.find(eventId);
		if (subs != subscribers.end()) {
			auto& handlers = subscribers[eventId];
			//Add new handler
			handlers.push_back(std::move(handler));
		} else {
			//Add new event id to subscribers
			subscribers[eventId].emplace_back(std::move(handler));
		}
		return handler->GetType();
	}

	void EventManager::Unsubscribe(const std::string eventId, const Utilities::ID handler) {
		auto handlers = subscribers[eventId];
		for(size_t i = 0; i < handlers.size(); i++) {
			if (handlers[i]->GetType() == handler) {
				delete handlers[i];
				handlers.erase(handlers.begin() + i);
				return;
			}
		}
	}

	void EventManager::Publish(const Event& event) {
		auto handlers = subscribers[event.GetEventType()];
		for (auto& handler : handlers) {
			handler->Execute(event);
		}
		//Log::Debug(event.GetEventType() + " published to " + std::to_string(handlers.size()) + " listeners.", true);
	}
}
