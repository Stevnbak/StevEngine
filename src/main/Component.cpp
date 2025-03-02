#include "Component.hpp"
#include "main/EngineEvents.hpp"
#include "main/GameObject.hpp"
#include "main/SceneManager.hpp"
#include "main/Log.hpp"

using namespace StevEngine::Utilities;

namespace StevEngine {
	Component::Component() {}

	void Component::SetObject(GameObject* object, std::string scene) {
		gameObject = object->Id();
		this->scene = scene;
		//Event listeners
		handlers.emplace_back(object->Subscribe<UpdateEvent>([this] (UpdateEvent e) { this->Update(e.deltaTime); }, false), UpdateEvent::GetStaticEventType());
		#ifdef StevEngine_SHOW_WINDOW
		handlers.emplace_back(object->Subscribe<DrawEvent>([this] (DrawEvent e) { this->Draw(e.transform); }, false), DrawEvent::GetStaticEventType());
		#endif
		handlers.emplace_back(object->Subscribe<DeactivateEvent>([this] (DeactivateEvent) { this->Deactivate(); }, false), DeactivateEvent::GetStaticEventType());
	}
	GameObject* Component::GetParent() const {
		return GetScene()->GetObject(gameObject);
	}
	Scene* Component::GetScene() const {
		return sceneManager.GetScene(scene);
	}
	Component::~Component() {
		//Unsubscribe from events
		if(handlers.empty()) return;
		GameObject* object = GetParent();
		if(!object) return;
		for(auto[id, event] : handlers) object->Unsubscribe(event, id);
	}
	//Export/Import component
	Component* CreateComponents::Create(const std::string& type, Utilities::Stream& stream) {
		if(!factories.contains(type)) {
			Log::Error("Component of type \"" + type + "\" is not registered as text importable.", true);
			return nullptr;
		}
		return factories[type](stream);
	}
}
