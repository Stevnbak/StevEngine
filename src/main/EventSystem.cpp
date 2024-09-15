#include "EventSystem.hpp"
#include "main/Log.hpp"
#include <memory>

namespace StevEngine {
	std::string EventManager::Subscribe(const std::string eventId, std::unique_ptr<EventHandlerBase> handler) {
		std::string name = handler->GetType();
		auto subs = subscribers.find(eventId);
		if (subs != subscribers.end()) {
			auto& handlers = subscribers[eventId];
			//Is handler already subscribed?
			for (std::unique_ptr<EventHandlerBase>& it : handlers) {
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
		auto& handlers = subscribers[eventId];
		for (auto it = handlers.begin(); it != handlers.end(); ++it) {
			if (it->get()->GetType() == handlerName) {
				it = handlers.erase(it);
				return;
			}
		}
	}

	void EventManager::Publish(const Event& event) {
		//Log::Debug("Event published: " + event.GetEventType(), true);
		for (auto& handler : subscribers[event.GetEventType()]) {
			handler->Execute(event);
		}
	}
}
