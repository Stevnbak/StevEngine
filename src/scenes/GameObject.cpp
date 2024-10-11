#include "GameObject.hpp"
#include "main/DataManager.hpp"
#include "main/EngineEvents.hpp"
#include "main/EventSystem.hpp"
#include "main/Log.hpp"
#include "scenes/SceneManager.hpp"
#include "scenes/Component.hpp"
#include "utilities/ID.hpp"
#include "utilities/Quaternion.hpp"
#include "utilities/Vector3.hpp"

#include <yaml-cpp/yaml.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <format>
#include <map>
#include <list>
#include <stdexcept>
#include <type_traits>
#include <iostream>

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
	void GameObject::Draw(glm::mat4x4 transform) {
		//Move
		transform = glm::translate(transform, glm::vec3(position.X, position.Y, position.Z));
		//Rotate
		transform *= glm::mat4_cast(glm::quat(rotation.W,rotation.X,rotation.Y,rotation.Z));
		//Scale
		transform = glm::scale(transform, glm::vec3(scale.X, scale.Y, scale.Z));
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
			return ((object->GetRotation() * object->GetWorldPosition()) + position);
		} else {
			return position;
		}
	}
	Utilities::Quaternion GameObject::GetWorldRotation() {
		if(!parent.IsNull()) {
			return sceneManager.GetScene(scene)->GetObject(parent)->GetWorldRotation() + rotation;
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
		children.emplace_back(id);
		sceneManager.GetScene(scene)->GetObject(id)->parent = this->id;
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
	int GameObject::GetChildCount() const {
		return children.size();
	}
	GameObject* GameObject::GetParent() const {
		if(!parent.IsNull()) return sceneManager.GetScene(scene)->GetObject(parent);
		else return nullptr;
	}

	//Export
	YAML::Node GameObject::Export() const {
		YAML::Node node;
		//Basic info
		node["id"] = id;
		node["name"] = name;
		//Transform info
		node["position"] = position;
		node["rotation"] = rotation;
		node["scale"] = scale;
		//Components
		for(Component* component : components) {
			node["components"].push_back(component->Export());
		}
		//Children
		for(int i = 0; i < children.size(); i++) {
			node["children"].push_back(GetChild(i)->Export());
		}
		return node;
	}
	#ifdef StevEngine_PLAYER_DATA
	void GameObject::ExportToFile(std::string name) const {
		YAML::Node node = Export();
		YAML::Emitter out;
		out << node;
		std::ofstream file;
		file.open(data.GetAppdataPath() + name + ".object");
		file << out.c_str();
	}
	#endif
	//Import from yaml
	void GameObject::Import(YAML::Node node)
	{
		//Basic info
		name = node["name"].as<std::string>();
		//Transform info
		position = node["position"].as<Utilities::Vector3>();
		rotation = node["rotation"].as<Utilities::Quaternion>();
		scale = node["scale"].as<Utilities::Vector3>();
		//Components
		if(node["components"]) {
			for(YAML::Node component : node["components"]) {
				AddComponent(CreateComponents::Create(component));
			}
		}
		//Children
		if(node["children"]) {
			for(YAML::Node childNode : node["children"]) {
				AddChild(sceneManager.GetScene(scene)->CreateObject(childNode));
			}
		}
	}
	//Destroy
	GameObject::~GameObject() {
		///Log::Normal(std::format("Destroying object with id {}", id), true);
		//Destroy children
		for (int i = 0; i < children.size(); i++) {
			sceneManager.GetScene(scene)->DestroyObject(children[i]);
		}
		//Destroy components
		for (int i = 0; i < components.size(); i++)
		{
			delete components[i];
		}
	}
}
