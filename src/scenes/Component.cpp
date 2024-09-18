#include "Component.hpp"
#include "main/EngineEvents.hpp"
#include "scenes/GameObject.hpp"
#include "scenes/SceneManager.hpp"
#include "main/Log.hpp"
#include <cstddef>
#include <stdexcept>

using namespace StevEngine::Utilities;

namespace StevEngine {
	Component::Component(std::string type) : type(type) {}
	void Component::SetObject(GameObject* object, std::string scene) {
		gameObject = object->Id();
		this->scene = scene;
		//Event listeners
		handlers.push_back({object->Subscribe<UpdateEvent>([this] (UpdateEvent e) { this->Update(e.deltaTime); }), UpdateEvent::GetStaticEventType()});
		handlers.push_back({object->Subscribe<DrawEvent>([this] (DrawEvent e) { this->Draw(e.transform);  }), DrawEvent::GetStaticEventType()});
		handlers.push_back({object->Subscribe<DeactivateEvent>([this] (DeactivateEvent) { this->Deactivate();  }), DeactivateEvent::GetStaticEventType()});
	}
	GameObject* Component::GetParent() {
		return GetScene()->GetObject(gameObject);
	}
	Scene* Component::GetScene() {
		return sceneManager.GetScene(scene);
	}
	Component::~Component() {
		//Unsubscribe from events
		GameObject* object = GetParent();
		for(auto[id, event] : handlers) {
			object->Unsubscribe(event, id);
		}
	}
	//Export/Import component
	YAML::Node Component::Export() const {
		YAML::Node node;
		node["type"] = type;
		return Export(node);
	}
	YAML::Node Component::Export(YAML::Node node) const {
		return node;
	}
	Component::Component(YAML::Node node) {
		type = node["type"].as<std::string>();
	}
	Component* CreateComponents::Create(YAML::Node node) {
		std::string type = node["type"].as<std::string>();
		if(!factories.contains(type)) {
			Log::Error("Component of type \"" + type + "\" is not registered as importable.", true);
			return nullptr;
		}
		return factories[type](node);
	}
}
