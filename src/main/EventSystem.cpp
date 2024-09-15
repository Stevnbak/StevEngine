#include "EventSystem.hpp"
#include "main/Log.hpp"
#include <memory>

namespace StevEngine {
	std::string EventManager::Subscribe(const std::string eventId, EventHandlerBase* handler) {
		std::string name = handler->GetType();
		auto subs = subscribers.find(eventId);
		if (subs != subscribers.end()) {
			auto& handlers = subscribers[eventId];
			//Is handler already subscribed?
			for (EventHandlerBase* it : handlers) {
				if (it->GetType() == name) {
					Log::Error("Attempting to double-register event handler", true);
					return nullptr;
				}
			}
			//Add new handler
			handlers.push_back(std::move(handler));
		} else {
			//Add new event id to subscribers
			subscribers[eventId].emplace_back(std::move(handler));
		}
		return name;
	}

	void EventManager::Unsubscribe(const std::string eventId, const std::string handlerName) {
		auto handlers = subscribers[eventId];
		for(size_t i = 0; i < handlers.size(); i++) {
			if (handlers[i]->GetType() == handlerName) {
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
