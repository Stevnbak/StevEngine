#include "Scene.hpp"
#include <format>
#include <regex>
#include "main/Engine.hpp"
#include "utilities/ID.hpp"

using namespace StevEngine::Utilities;

namespace StevEngine {
	ID Scene::CreateObject() {
		ID id;
		gameObjects.insert(std::make_pair(id, GameObject(id, "GameObject", this->name)));
		if(active) gameObjects.at(id).Start();
		return id;
	}
	ID Scene::CreateObject(std::string name, Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale) {
		ID id;
		gameObjects.insert(std::make_pair(id, GameObject(id, name, this->name)));
		GameObject* object = GetObject(id);
		object->position = position;
		object->rotation = rotation;
		object->scale = scale;
		if(active) object->Start();
		return id;
	}
	ID Scene::CreateObjectFromFile(Resources::Resource file) {
		tinyxml2::XMLDocument doc;
		doc.Parse(file.GetStrData().c_str());
		tinyxml2::XMLElement* elem = doc.FirstChildElement();
		return CreateObjectFromXML(doc.FirstChildElement());
	}
	ID Scene::CreateObjectFromXML(tinyxml2::XMLElement* node) {
		//Create base object:
		ID id = ID(node->Attribute("id"));
		gameObjects.insert(std::make_pair(id, GameObject(id, node->Attribute("name"), this->name)));
		GameObject* object = GetObject(id);
		object->position = Utilities::Vector3(node->Attribute("position"));
		object->rotation = Utilities::Quaternion(node->Attribute("rotation"));
		object->scale = Utilities::Vector3(node->Attribute("scale"));
		//Add child objects & components
		tinyxml2::XMLElement* child = node->FirstChildElement();
		for(int i = 0; i < node->ChildElementCount(); i++) {
			std::string name = child->Name();
			if(name == "GameObject") {
				object->AddChild(CreateObjectFromXML(child));
			} else if(name == "Component") {
				std::string test = child->Attribute("type");
				FactoryBase* factory = GameObject::componentFactories.at(test);
				object->AddComponent(factory->create(child));
			}
			child = child->NextSiblingElement();
		}
		//Run start
		if(active) object->Start();
		//Return
		return object->id;
	}
	std::vector<ID> Scene::GetAllObjects() {
		std::vector<ID> keys;
		for(std::map<ID, GameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it) {
			keys.push_back(it->first);
		}
		return keys;
	}
	void Scene::DestroyObject(ID id) {
		gameObjects.erase(id);
	}
    Scene::Scene(tinyxml2::XMLElement* node) : name(node->Attribute("name")) {}
	Scene::Scene(std::string name) : name(name) {
        //Create main camera
		activeCamera = GetObject(CreateObject("Main Camera"))->AddComponent(new Camera(false, 1, 16 / 9));
    }
	void Scene::ExportToFile() {
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLElement* main = doc.NewElement("Scene");
		doc.InsertFirstChild(main);
		main->SetAttribute("name", name.c_str());
		main->SetAttribute("camera", activeCamera->GetParent()->Id().GetString().c_str());

		for (ID id : GetAllObjects()) {
			GameObject* object = &gameObjects.at(id);
			bool test = object->parent.IsNull();
			if(!object->parent.IsNull()) continue;
			tinyxml2::XMLDocument xml;
			xml.Parse(object->Export().c_str());
			tinyxml2::XMLElement* element = xml.FirstChild()->ToElement();
			main->InsertEndChild(element->DeepClone(&doc));
		}

		doc.SaveFile((Engine::Instance->data.GetDirectoryPath() + std::regex_replace(name, std::regex(" "), "_") + ".scene").c_str());
	}
	void Scene::Activate() {
		active = true;
		for (ID id : GetAllObjects()) {
			gameObjects.at(id).Start();
		}
	}
	void Scene::Deactivate() {
		active = false;
		for (ID id : GetAllObjects()) {
			gameObjects.at(id).Deactivate();
		}
	}
}