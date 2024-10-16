#include "SceneManager.hpp"
#include "main/Engine.hpp"
#include "main/EngineEvents.hpp"
#include "utilities/ID.hpp"

#include <yaml-cpp/yaml.h>

namespace StevEngine {
	SceneManager sceneManager = SceneManager();
	//Scene manager
	void SceneManager::SetActiveScene(std::string name) {
		if(active != "") GetScene(active)->Deactivate();
		active = name;
		if(engine->running) GetScene(active)->Activate();
	}
	void SceneManager::ActivateDefault() {
		if(scenes.size() == 0) CreateScene("Default");
		if(active == "") SetActiveScene(scenes.begin()->first);
		GetActiveScene()->Activate();
	}

	void SceneManager::Init() {
		engine->GetEvents()->Subscribe<EngineStartEvent>([this] (EngineStartEvent) { this->ActivateDefault(); });
		engine->GetEvents()->Subscribe<UpdateEvent>([this] (UpdateEvent e) { this->Update(e.deltaTime); });
		#ifdef StevEngine_SHOW_WINDOW
		engine->GetEvents()->Subscribe<EngineDrawEvent>([this] (EngineDrawEvent) { this->Draw(); });
		#endif
	}

	void SceneManager::Update(double deltaTime) {
		Scene* scene = GetActiveScene();
		for (Utilities::ID id : scene->GetAllParentObjects()) {
			scene->GetObject(id)->Update(deltaTime);
		}
	}
	#ifdef StevEngine_SHOW_WINDOW
	void SceneManager::Draw() {
		Scene* scene = sceneManager.GetActiveScene();
		if (scene->activeCamera == nullptr) return;
		for (Utilities::ID id : scene->GetAllParentObjects()) {
			scene->GetObject(id)->Draw(glm::mat4x4(1.0));
		}
	}
	#endif

	Scene* SceneManager::CreateScene(std::string name) {
		if(scenes.contains(name)) throw std::runtime_error("Scene \"" + name + "\" already exists!");
		scenes.emplace(name, Scene(name));
		return &scenes.at(name);
	}

	Scene* SceneManager::CreateSceneFromFile(Resources::Resource file) {
		YAML::Node node = YAML::Load(file.GetRawData());
		std::string name = node["name"].as<std::string>();
		if(scenes.contains(name)) throw std::runtime_error("Scene \"" + name + "\" already exists!");
		scenes.emplace(name, Scene(node));
		return &scenes.at(name);
	}
}
