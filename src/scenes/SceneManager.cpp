#include "SceneManager.hpp"
#include "utilities/ID.hpp"
#include "yaml-cpp/node/node.h"
#include "yaml-cpp/node/parse.h"

namespace StevEngine {
	//Scene manager
	void SceneManager::SetActiveScene(std::string name) {
		if(active != "") GetScene(active)->Deactivate();
		active = name;
		GetScene(active)->Activate();
	}
	void SceneManager::ActivateDefault() {
		if(scenes.size() == 0) CreateScene("Default");
		if(active == "") SetActiveScene(scenes.begin()->first);
	}

	SceneManager::SceneManager() : active("") {

	}

	Scene* SceneManager::CreateScene(std::string name) {
		if(scenes.contains(name)) throw std::runtime_error("Scene \"" + name + "\" already exists!");
		scenes.insert(std::make_pair(name, Scene(name)));
		return &scenes.at(name);
	}

	Scene* SceneManager::CreateSceneFromFile(Resources::Resource file) {
		YAML::Node node = YAML::Load(file.GetRawData());
		std::string name = node["name"].as<std::string>();
		if(scenes.contains(name)) throw std::runtime_error("Scene \"" + name + "\" already exists!");
		scenes.insert(std::make_pair(name, Scene(node)));
		return &scenes.at(name);
	}
}
