#include "GameObject.hpp"
#include "main/EngineEvents.hpp"
#include "main/EventSystem.hpp"
#include "main/SceneManager.hpp"
#include "main/Component.hpp"
#include "utilities/ID.hpp"
#include "utilities/Matrix4.hpp"
#include "utilities/Quaternion.hpp"
#include "utilities/Stream.hpp"
#include "utilities/Vector3.hpp"

#include <cstdio>

namespace StevEngine {
	//Main functions
	void GameObject::Start() {
		for (Component* component : components) {
			component->Start();
		}
	}
	void GameObject::Deactivate() {
		events.Publish(DeactivateEvent());
	}
	void GameObject::Update(double deltaTime) {
		events.Publish(UpdateEvent(deltaTime));
	}
	#ifdef StevEngine_SHOW_WINDOW
	void GameObject::Draw(Utilities::Matrix4 transform) {
		//New transform
		transform = Utilities::Matrix4::FromTranslationRotationScale(position, rotation, scale) * transform;
		//Event
		events.Publish(DrawEvent(transform));
	}
	#endif

	//Transform
	Utilities::Vector3 GameObject::GetPosition() const { return position; }
	Utilities::Quaternion GameObject::GetRotation() const { return rotation; }
	Utilities::Vector3 GameObject::GetScale() const { return scale; }
	void GameObject::SetPosition(Utilities::Vector3 position, bool announce) {
		this->position = position;
		if(announce) events.Publish(TransformUpdateEvent(true, false, false));
	}
	void GameObject::SetRotation(Utilities::Quaternion rotation, bool announce) {
		this->rotation = rotation;
		if(announce) events.Publish(TransformUpdateEvent(false, true, false));
	}
	void GameObject::SetScale(Utilities::Vector3 scale, bool announce) {
		this->scale = scale;
		if(announce) events.Publish(TransformUpdateEvent(false, false, true));
	}
	void GameObject::SetTransform(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale, bool announce) {
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
		if(announce) events.Publish(TransformUpdateEvent(true, true, true));
	}
	Utilities::Vector3 GameObject::GetWorldPosition() {
		if(!parent.IsNull()) {
			GameObject* object = GetParent();
			return (Utilities::Matrix4::FromTranslationRotationScale(object->GetWorldPosition(), object->GetRotation(), object->GetScale()) * position);
		} else {
			return position;
		}
	}
	Utilities::Quaternion GameObject::GetWorldRotation() {
		if(!parent.IsNull()) {
			return sceneManager.GetScene(scene)->GetObject(parent)->GetWorldRotation() * rotation;
		} else {
			return rotation;
		}
	}
	Utilities::Vector3 GameObject::GetWorldScale() {
		if(!parent.IsNull()) {
			return Utilities::Vector3::CombineScale(GetParent()->GetWorldScale(), scale);
		} else {
			return scale;
		}
	}

	//Constructors
	GameObject::GameObject() : id(Utilities::ID()), name("GameObject") {
		//Log::Normal(std::format("Creating gameobject with new id {}", id.GetString()), true);
	}
	GameObject::GameObject(Utilities::ID id, std::string name, std::string scene) : id(id), name(name), scene(scene) {
		//Log::Normal(std::format("Creating gameobject with id {}", id.GetString()), true);
	}

	//Children functions
	int GameObject::AddChild(const Utilities::ID& id) {
		children.emplace_back(id);
		sceneManager.GetScene(scene)->GetObject(id)->SetParent(this->id);
		int i = children.size() - 1;
		return i;
	}
	void GameObject::RemoveChild(int index) {
		GetChild(index)->parent = Utilities::ID::empty;
		children.erase(children.begin() + index);
	}
	GameObject* GameObject::GetChild(int index) const {
		return sceneManager.GetScene(scene)->GetObject(children[index]);
	}
	uint GameObject::GetChildCount() const {
		return children.size();
	}
	GameObject* GameObject::GetParent() const {
		if(!parent.IsNull()) return sceneManager.GetScene(scene)->GetObject(parent);
		else return nullptr;
	}
	void GameObject::SetParent(const Utilities::ID& id) {
		if(id.IsNull()) return;
		if(!parent.IsNull()) {
			GameObject* p = GetParent();
			for(auto[id, event] : handlers) p->Unsubscribe(event, id);
		}
		GameObject* object = sceneManager.GetScene(scene)->GetObject(id);
		handlers.emplace_back(object->Subscribe<UpdateEvent>([this] (UpdateEvent e) { this->Update(e.deltaTime); }), UpdateEvent::GetStaticEventType());
		#ifdef StevEngine_SHOW_WINDOW
		handlers.emplace_back(object->Subscribe<DrawEvent>([this] (DrawEvent e) { this->Draw(e.transform);  }), DrawEvent::GetStaticEventType());
		#endif
		handlers.emplace_back(object->Subscribe<DeactivateEvent>([this] (DeactivateEvent) { this->Deactivate();  }), DeactivateEvent::GetStaticEventType());
		parent = id;
	}

	//Export to stream
	Utilities::Stream GameObject::Export(Utilities::StreamType type) const {
		Utilities::Stream stream(type);
		//Basic info
		stream << id << name;
		stream << position << rotation << scale;
		//Components and children
		stream << (uint)components.size() << GetChildCount();
		for(auto component : components) {
			stream << component->GetType() << component->Export(type);
		}
		for(int i = 0; i < GetChildCount(); i++) {
			stream << GetChild(i)->Export(type);
		}
		return stream;
	}
	//Import from stream
	void GameObject::Import(Utilities::Stream& stream) {
		//Basic info
		stream >> name >> position >> rotation >> scale;
		//Components and children
		uint components, children;
		stream >> components >> children;
		for(int i = 0; i < components; i++) {
			std::string type;
			stream >> type;
			AddComponent(CreateComponents::Create(type, stream));
		}
		for(int i = 0; i < children; i++) {
			sceneManager.GetScene(scene)->CreateObject(stream);
		}
	}
	//Destroy
	GameObject::~GameObject() {
		///Log::Normal(std::format("Destroying object with id {}", id), true);
		//Remove event listeners
		GameObject* p = GetParent();
		for(auto[id, event] : handlers) p->Unsubscribe(event, id);
		//Destroy children
		for (int i = 0; i < children.size(); i++) {
			sceneManager.GetScene(scene)->DestroyObject(children[i]);
		}
		//Destroy components
		for (int i = 0; i < components.size(); i++)
		{
			components[i]->handlers.clear();
			delete components[i];
		}
	}
}
