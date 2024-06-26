#include "GameObject.hpp"
#include <main/Log.hpp>
#include <main/Engine.hpp>

#include <glad/glad.h>
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
		for (size_t i = 0; i < components.size(); i++) {
			components.at(i)->Deactivate();
		}
	}
	void GameObject::Update(double deltaTime) {
		//Components
		for (size_t i = 0; i < components.size(); i++) {
			components.at(i)->Update(deltaTime);
		}
	}
	void GameObject::Draw(glm::mat4x4 transform) {
		//Move
		transform = glm::translate(transform, glm::vec3(position.X, position.Y, position.Z));
		//Rotate
		transform *= glm::mat4_cast(glm::quat(rotation.W,rotation.X,rotation.Y,rotation.Z));
		//Scale
		transform = glm::scale(transform, glm::vec3(scale.X, scale.Y, scale.Z));
		//Components
		for (int i = 0; i < components.size(); i++) {
			components.at(i)->Draw(transform);
		}
		//Children
		for (int i = 0; i < children.size(); i++) {
			GetChild(i)->Draw(transform);
		}
	}

	//Transform
	Utilities::Vector3 GameObject::GetPosition() { return position; }
	Utilities::Quaternion GameObject::GetRotation() { return rotation; }
	Utilities::Vector3 GameObject::GetScale() { return scale; }
	void GameObject::SetPosition(Utilities::Vector3 position, bool announce) {
		this->position = position;
		if(announce)
			TransformUpdate(true, false, false);
	}
	void GameObject::SetRotation(Utilities::Quaternion rotation, bool announce) {
		this->rotation = rotation;
		if(announce)
			TransformUpdate(false, true, false);
	}
	void GameObject::SetScale(Utilities::Vector3 scale, bool announce) {
		this->scale = scale;
		if(announce)
			TransformUpdate(false, false, true);
	}
	void GameObject::SetTransform(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale, bool announce) {
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
		if(announce)
			TransformUpdate(true, true, true);
	}
	void GameObject::TransformUpdate(bool position, bool rotation, bool scale) {
		for (int i = 0; i < components.size(); i++) {
			components[i]->TransformUpdate(position, rotation, scale);
		}
		for (int i = 0; i < children.size(); i++) {
			Engine::Instance->scenes.GetScene(scene)->GetObject(children[i])->TransformUpdate(position, rotation, scale);
		}
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
			return Engine::Instance->scenes.GetScene(scene)->GetObject(parent)->GetWorldRotation() + rotation;
		} else {
			return rotation;
		}
	}
	Utilities::Vector3 GameObject::GetWorldScale() {
		if(!parent.IsNull()) {
			return Utilities::Vector3::CombineScale(Engine::Instance->scenes.GetScene(scene)->GetObject(parent)->GetWorldScale(), scale);
		} else {
			return scale;
		}
	}

	//Constructors
	GameObject::GameObject() : id(Utilities::ID()), name("GameObject") {
		Log::Normal(std::format("Creating gameobject with new id {}", id.GetString()), true);
	}
	GameObject::GameObject(Utilities::ID id, std::string name, std::string scene) : id(id), name(name), scene(scene) {
		Log::Normal(std::format("Creating gameobject with id {}", id.GetString()), true);
	}

	//Children functions
	int GameObject::AddChild(Utilities::ID gameObject) {
		children.push_back(gameObject);
		Engine::Instance->scenes.GetScene(scene)->GetObject(gameObject)->parent = gameObject;
		return children.size() - 1;
	}
	void GameObject::RemoveChild(int index) {
		GetChild(index)->parent = Utilities::ID::empty;
		children.erase(children.begin() + index);
	}
	GameObject* GameObject::GetChild(int index) {
		return Engine::Instance->scenes.GetScene(scene)->GetObject(children[index]);
	}
	int GameObject::GetChildCount() {
		return children.size();
	}
	GameObject* GameObject::GetParent() {
		if(!parent.IsNull()) return Engine::Instance->scenes.GetScene(scene)->GetObject(parent);
		else return nullptr;
	}

	//Export
	std::string GameObject::Export() {
		//Create xml document
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLElement* main = doc.NewElement("GameObject");
		doc.InsertFirstChild(main);

		//Add basic info
		main->SetAttribute("name", 		name.c_str());
		main->SetAttribute("id", 		id.GetString().c_str());
		main->SetAttribute("position", 	((std::string)position).c_str());
		main->SetAttribute("rotation", 	((std::string)rotation).c_str());
		main->SetAttribute("scale", 	((std::string)scale).c_str());

		//Add components
		for(Component* component : components) {
			tinyxml2::XMLDocument xml;
			xml.Parse(component->Export().c_str());
			tinyxml2::XMLElement* element = xml.FirstChild()->ToElement();
			main->InsertEndChild(element->DeepClone(&doc));
		}

		//Add children
		for(Utilities::ID child : children) {
			tinyxml2::XMLDocument xml;
			xml.Parse(Engine::Instance->scenes.GetScene(scene)->GetObject(child)->Export().c_str());
			tinyxml2::XMLElement* element = xml.FirstChild()->ToElement();
			main->InsertEndChild(element->DeepClone(&doc));
		}

		//Return xml as string
		tinyxml2::XMLPrinter printer;
		doc.Print( &printer );
		return printer.CStr();
	}
	void GameObject::ExportToFile(std::string name) {
		tinyxml2::XMLDocument doc;
		doc.Parse(Export().c_str());
		doc.SaveFile((Engine::Instance->data.GetDirectoryPath() + name + ".object").c_str());
	}

	//Destroy
	GameObject::~GameObject() {
		///Log::Normal(std::format("Destroying object with id {}", id), true);
		//Destroy children
		for (int i = 0; i < children.size(); i++) {
			Engine::Instance->scenes.GetScene(scene)->DestroyObject(children[i]);
		}
		//Destroy components
		for (int i = 0; i < components.size(); i++)
		{
			delete components[i];
		}
	}
}