#include "GameObject.hpp"
#include <core/Log.hpp>
#include <core/Engine.hpp>

#include <SDL2/SDL_opengl.h>

#include <format>
#include <map>
#include <list>
#include <stdexcept>
#include <type_traits>
#include <iostream>

namespace StevEngine {
	//Static variables
	ID GameObject::currentID = 1;

	//Main functions
	void GameObject::Start() {
		for (int i = 0; i < components.size(); i++) {
			Component* component = components[i];
			component->Start();
		}
	}
	void GameObject::Deactivate() {
		for (int i = 0; i < components.size(); i++) {
			Component* component = components[i];
			component->Start();
		}
	}
	void GameObject::Update(double deltaTime) {
		//Components
		for (int i = 0; i < components.size(); i++) {
			Component* component = components[i];
			component->Update(deltaTime);
		}
	}
	void GameObject::Draw() {
		glPushMatrix();
			TransformView();
			//Components
			for (int i = 0; i < components.size(); i++) {
				Component* component = components[i];
				component->Draw();
			}
		glPopMatrix();
	}
	void GameObject::TransformView() {
		//Parent transform
		if (parent != 0) {
			Engine::Instance->scenes.GetScene(scene)->GetObject(parent)->TransformView();
		}
		//Position
		glTranslated(position.X, position.Y, position.Z);
		//Rotation
		std::tuple<double, Utilities::Vector3> angleAxis = rotation.GetAngleAxis();
		double angle = Utilities::Quaternion::RadiansToDegrees(std::get<0>(angleAxis));
		Utilities::Vector3 v = std::get<1>(angleAxis);
		glRotated(angle, v.X, v.Y, v.Z);
		//Scale
		glScaled(scale.X, scale.Y, scale.Z);
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
		if(parent != 0) {
			return Engine::Instance->scenes.GetScene(scene)->GetObject(parent)->GetWorldPosition() + position;
		} else {
			return position;
		}
	}
	Utilities::Quaternion GameObject::GetWorldRotation() {
		if(parent != 0) {
			return Engine::Instance->scenes.GetScene(scene)->GetObject(parent)->GetWorldRotation() + rotation;
		} else {
			return rotation;
		}
	}
	Utilities::Vector3 GameObject::GetWorldScale() {
		if(parent != 0) {
			return Engine::Instance->scenes.GetScene(scene)->GetObject(parent)->GetWorldScale() + scale;
		} else {
			return scale;
		}
	}

	//Constructors
	GameObject::GameObject() : id(GameObject::currentID++), name("GameObject_" + id) {
		Log::Normal(std::format("Creating gameobject with new id {}", id), true);
	}
	GameObject::GameObject(ID id, std::string name, std::string scene) : id(id), name(name), scene(scene) {
		Log::Normal(std::format("Creating gameobject with id {}", id), true);
	}

	//Children functions
	int GameObject::AddChild(ID gameObject) {
		children.push_back(gameObject);
		Engine::Instance->scenes.GetScene(scene)->GetObject(gameObject)->parent = gameObject;
		return children.size() - 1;
	}
	void GameObject::RemoveChild(int index) {
		GetChild(index)->parent = 0;
		children.erase(children.begin() + index);
	}
	GameObject* GameObject::GetChild(int index) {
		return Engine::Instance->scenes.GetScene(scene)->GetObject(children[index]);
	}
	int GameObject::GetChildCount() {
		return children.size();
	}
	GameObject* GameObject::GetParent() {
		if(parent != 0) return Engine::Instance->scenes.GetScene(scene)->GetObject(parent);
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
		for(ID child : children) {
			tinyxml2::XMLDocument xml;
			xml.Parse(Engine::Instance->scenes.GetScene(scene)->GetObject(child)->Export().c_str());
			main->InsertEndChild(xml.FirstChild());
		}

		//Return xml as string
		tinyxml2::XMLPrinter printer;
		doc.Print( &printer );
		return printer.CStr();
	}
	void GameObject::ExportToFile(std::string path) {
		tinyxml2::XMLDocument doc;
		doc.Parse(Export().c_str());
		doc.SaveFile((std::format("{}/{}/", "appdata", Engine::Instance->title) + path).c_str());
	}

	//Destroy
	GameObject::~GameObject() {
		///Log::Normal(std::format("Destroying object with id {}", id));
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