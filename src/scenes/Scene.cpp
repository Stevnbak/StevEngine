#include "Scene.hpp"
#include "main/DataManager.hpp"
#include "scenes/Component.hpp"
#include "utilities/ID.hpp"

#include <memory>
#include <yaml-cpp/yaml.h>

#include <format>
#include <regex>

using namespace StevEngine::Utilities;

namespace StevEngine {
	ID Scene::CreateObject() {
		ID id;
		Lock();
		gameObjects.try_emplace(id, GameObject(id, "GameObject", name));
		Unlock();
		if(active) GetObject(id)->Start();
		return id;
	}
	ID Scene::CreateObject(std::string name, Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale) {
		ID id;
		Lock();
		gameObjects.try_emplace(id, GameObject(id, name, this->name));
		Unlock();
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
		Lock();
		gameObjects.try_emplace(id, GameObject(id, "GameObject", name));
		Unlock();
		GameObject* object = GetObject(id);
		object->Import(node);
		if(active) object->Start();
		return id;
	}
	GameObject* Scene::GetObject(Utilities::ID id) {
		Lock();
		GameObject* obj;
		if(!gameObjects.contains(id)) obj = nullptr;
		else obj = &gameObjects.at(id);
		Unlock();
		return obj;
	}
	std::vector<ID> Scene::GetAllObjects() const {
		std::vector<ID> keys;
		for(auto&[id, object] : gameObjects) {
			keys.push_back(id);
		}
		return keys;
	}
	std::vector<ID> Scene::GetAllParentObjects() const {
		std::vector<ID> keys;
		for(auto&[id, object] : gameObjects) {
			if(object.parent.IsNull()) keys.push_back(id);
		}
		return keys;
	}
	void Scene::DestroyObject(ID id) {
		gameObjects.erase(id);
	}
	Scene::Scene(std::string name) : name(name) {
		//Create main camera
		#ifdef StevEngine_SHOW_WINDOW
		activeCamera = GetObject(CreateObject("Main Camera"))->AddComponent(new Visuals::Camera());
		#endif
	}
	Scene::Scene(YAML::Node node) : name(node["name"].as<std::string>()) {
		//Create objects
		for(YAML::Node obj : node["objects"]) {
			CreateObject(obj);
		}
		//Set camera
		#ifdef StevEngine_SHOW_WINDOW
		activeCamera = GetObject(node["camera"].as<Utilities::ID>())->GetComponent<Visuals::Camera>();
		#endif
	}
	#ifdef StevEngine_PLAYER_DATA
	void Scene::ExportToFile() const {
		YAML::Node node;
		node["name"] = name;
		#ifdef StevEngine_SHOW_WINDOW
		node["camera"] = activeCamera->GetParent()->Id();
		#endif

		for(auto&[id, object] : gameObjects) {
			if(!object.parent.IsNull()) continue;
			node["objects"].push_back(object.Export());
		}

		YAML::Emitter out;
		out << node;
		std::ofstream file;
		file.open(data.GetAppdataPath() + name + ".scene");
		file << out.c_str();
	}
	#endif
	void Scene::Activate() {
		Lock();
		active = true;
		Unlock();
		for (auto id : GetAllObjects()) {
			GetObject(id)->Start();
		}
	}
	void Scene::Deactivate() {
		Lock();
		active = false;
		Unlock();
		for (auto id : GetAllObjects()) {
			GetObject(id)->Deactivate();
		}
	}
}
