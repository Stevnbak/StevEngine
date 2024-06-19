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
	std::map<ID, GameObject> GameObject::gameObjects;

	//Main functions
	void GameObject::Start() {
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
			gameObjects.at(parent).TransformView();
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
			gameObjects.at(children[i]).TransformUpdate(position, rotation, scale);
		}
	}
	Utilities::Vector3 GameObject::GetWorldPosition() {
		if(parent != 0) {
			return gameObjects.at(parent).GetWorldPosition() + position;
		} else {
			return position;
		}
	}
	Utilities::Quaternion GameObject::GetWorldRotation() {
		if(parent != 0) {
			return gameObjects.at(parent).GetWorldRotation() + rotation;
		} else {
			return rotation;
		}
	}
	Utilities::Vector3 GameObject::GetWorldScale() {
		if(parent != 0) {
			return gameObjects.at(parent).GetWorldScale() + scale;
		} else {
			return scale;
		}
	}

	//Constructors
	GameObject::GameObject() : id(GameObject::currentID++), name("GameObject_" + id) {
		Log::Normal(std::format("Creating gameobject with new id {}", id), true);
	}
	GameObject::GameObject(ID id, std::string name) : id(id), name(name) {
		Log::Normal(std::format("Creating gameobject with id {}", id), true);
	}
	ID GameObject::Create() {
		ID id = GameObject::currentID++;
		gameObjects.insert(std::make_pair(id, GameObject(id, "GameObject_" + id)));
		gameObjects.at(id).Start();
		return id;
	}
	ID GameObject::Create(std::string name, Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale) {
		ID id = GameObject::currentID++;
		gameObjects.insert(std::make_pair(id, GameObject(id, name)));
		GameObject* object = GetObject(id);
		object->position = position;
		object->rotation = rotation;
		object->scale = scale;
		object->Start();
		return id;
	}
	ID GameObject::CreateFromFile(Resources::Resource file) {
		tinyxml2::XMLDocument doc;
		doc.Parse(file.GetStrData().c_str());
		tinyxml2::XMLElement* elem = doc.FirstChildElement();
		return CreateFromXML(doc.FirstChildElement());
	}
	ID GameObject::CreateFromXML(tinyxml2::XMLElement* node) {
		//Create base object:
		ID id = Create(node->Attribute("name"), Utilities::Vector3(node->Attribute("position")), Utilities::Quaternion(node->Attribute("rotation")), Utilities::Vector3(node->Attribute("scale")));
		GameObject* object = GetObject(id);
		//Add child objects & components
		tinyxml2::XMLElement* child = node->FirstChildElement();
		for(int i = 0; i < node->ChildElementCount(); i++) {
			std::string name = child->Name();
			if(name == "GameObject") {
				object->AddChild(CreateFromXML(child));
			} else if(name == "Component") {
				std::string test = child->Attribute("type");
				FactoryBase* factory = componentFactories.at(test);
				object->AddComponent(factory->create(child));
			}
			child = child->NextSiblingElement();
		}
		//Add to list
		object->Start();
		//Return
		return id;
	}
	std::vector<ID> GameObject::GetAllObjects() {
		std::vector<ID> keys;
		for(std::map<ID, GameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it) {
			keys.push_back(it->first);
		}
		return keys;
	}

	
	//Children functions
	int GameObject::AddChild(ID gameObject) {
		children.push_back(gameObject);
		gameObjects.at(gameObject).parent = gameObject;
		return children.size() - 1;
	}
	void GameObject::RemoveChild(int index) {
		gameObjects.at(GetChild(index)).parent = 0;
		children.erase(children.begin() + index);
	}
	ID GameObject::GetChild(int index) {
		return children[index];
	}
	int GameObject::GetChildCount() {
		return children.size();
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
			xml.Parse(gameObjects.at(child).Export().c_str());
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
	//Export component
	std::string Component::Export() {
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLElement* main = doc.NewElement("Component");
		doc.InsertFirstChild(main);
		//Add basic info
		main->SetAttribute("type", type.c_str());

		//Add component specific info
		this->Export(main);
		
		//Return xml as string
		tinyxml2::XMLPrinter printer;
		doc.Print( &printer );
		return printer.CStr();
	}

	//Destroy
	GameObject::~GameObject() {
		///Log::Normal(std::format("Destroying object with id {}", id));
		//Destroy children
		for (int i = 0; i < children.size(); i++) {
			delete GameObject::GetObject(children[i]);
		}
		//Destroy components
		for (int i = 0; i < components.size(); i++)
		{
			delete components[i];
		}
	}
}