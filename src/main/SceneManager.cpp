#include "SceneManager.hpp"
#include "main/Engine.hpp"
#include "main/EngineEvents.hpp"
#include "utilities/Stream.hpp"
#include <cassert>

namespace StevEngine {
	SceneManager sceneManager = SceneManager();

	bool SceneManager::Exists(std::string name) {
		return scenes.contains(name);
	}

	Scene& SceneManager::GetScene(std::string name) {
		assert(scenes.contains(name) && "No scene found.");
		return scenes.at(name);
	}

	Scene& SceneManager::GetActiveScene() {
		assert(scenes.contains(active) && active != "" && "Active scene not found.");
		return scenes.at(active);
	}

	void SceneManager::SetActiveScene(std::string name) {
		if(active != "") GetScene(active).Deactivate();
		active = name;
		if(engine->running) GetScene(active).Activate();
	}
	void SceneManager::ActivateDefault() {
		if(scenes.size() == 0) CreateScene("Default");
		if(active == "") SetActiveScene(scenes.begin()->first);
		GetActiveScene().Activate();
	}

	void SceneManager::Init() {
		EventManager& events = engine->GetEvents();
		events.Subscribe<EngineStartEvent>([this] (EngineStartEvent) { this->ActivateDefault(); });
		events.Subscribe<UpdateEvent>([this] (UpdateEvent e) { this->Update(e.deltaTime); });
		#ifdef StevEngine_SHOW_WINDOW
		events.Subscribe<EngineDrawEvent>([this] (EngineDrawEvent) { this->Draw(); });
		#endif
		events.Subscribe<EngineQuitEvent>([this](EngineQuitEvent e) {
			scenes.clear();
		});
	}

	void SceneManager::Update(double deltaTime) {
		Scene& scene = GetActiveScene();
		for (Utilities::ID id : scene.GetAllParentObjects()) {
			scene.GetObject(id).Update(deltaTime);
		}
	}
	#ifdef StevEngine_SHOW_WINDOW
	void SceneManager::Draw() {
		Scene& scene = sceneManager.GetActiveScene();
		if (scene.activeCamera == nullptr) return;
		for (Utilities::ID id : scene.GetAllParentObjects()) {
			scene.GetObject(id).Draw(Utilities::Matrix4::identity);
		}
	}
	#endif

	Scene& SceneManager::CreateScene(std::string name) {
		if(scenes.contains(name)) throw std::runtime_error("Scene \"" + name + "\" already exists!");
		scenes.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(name));
		return scenes.at(name);
	}

	Scene& SceneManager::CreateSceneFromFile(Resources::Resource file, Utilities::StreamType type) {
		Utilities::Stream stream(type);
		stream.ReadFromFile(file);
		std::string name;
		stream >> name;
		if(scenes.contains(name)) throw std::runtime_error("Scene \"" + name + "\" already exists!");
		scenes.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(name, stream));
		return scenes.at(name);
	}
}
