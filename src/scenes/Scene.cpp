#include "Scene.hpp"
#include <format>
#include <regex>
#include "main/Engine.hpp"
#include "scenes/Component.hpp"
#include "utilities/ID.hpp"
#include "yaml-cpp/node/node.h"
#include "yaml-cpp/node/parse.h"

using namespace StevEngine::Utilities;

namespace StevEngine {
	ID Scene::CreateObject() {
		ID id;
		gameObjects.insert(std::make_pair(id, GameObject(id, "GameObject", name)));
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
	Utilities::ID Scene::CreateObject(Resources::Resource file) {
		YAML::Node node = YAML::Load(file.GetRawData());
		return CreateObject(node);
	}
	Utilities::ID Scene::CreateObject(YAML::Node node) {
		Utilities::ID id = node["id"] ? node["id"].as<Utilities::ID>() : Utilities::ID();
		gameObjects.insert(std::make_pair(id, GameObject(id, "GameObject", name)));
		GameObject* object = &gameObjects.at(id);
		object->Import(node);
		if(active) object->Start();
		return id;
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
	Scene::Scene(std::string name) : name(name) {
		//Create main camera
		activeCamera = GetObject(CreateObject("Main Camera"))->AddComponent(new Visuals::Camera(false, 1));
	}
	Scene::Scene(YAML::Node node) : name(node["name"].as<std::string>()) {
		//Create objects
		for(YAML::Node obj : node["objects"]) {
			CreateObject(obj);
		}
		//Set camera
		activeCamera = GetObject(node["camera"].as<Utilities::ID>())->GetComponent<Visuals::Camera>();
	}
	#ifdef StevEngine_PLAYER_DATA
	void Scene::ExportToFile() {
		YAML::Node node;
		node["name"] = name;
		node["camera"] = activeCamera->GetParent()->Id();

		for (ID id : GetAllObjects()) {
			GameObject* object = &gameObjects.at(id);
			if(!object->parent.IsNull()) continue;
			node["objects"].push_back(object->Export());
		}

		YAML::Emitter out;
		out << node;
		std::ofstream file;
		file.open(Engine::Instance->data.GetDirectoryPath() + name + ".scene");
		file << out.c_str();
	}
	#endif
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
