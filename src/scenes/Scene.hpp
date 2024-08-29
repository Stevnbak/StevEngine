#pragma once
#include "main/ResourceManager.hpp"
#include "visuals/Camera.hpp"
#include "scenes/GameObject.hpp"
#include "yaml-cpp/node/node.h"

namespace StevEngine {
	class Engine;
	class SceneManager;

	class Scene {
		friend class Engine;
		friend class SceneManager;
		public:
			const std::string name;
		public:
			Utilities::ID CreateObject();
			Utilities::ID CreateObject(std::string name, Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1, 1, 1));
			Utilities::ID CreateObject(Resources::Resource file);
			Utilities::ID CreateObject(YAML::Node node);
			GameObject* GetObject(Utilities::ID id) { return &gameObjects.at(id); }
			Visuals::Camera* GetCamera() { return activeCamera; }
			GameObject* GetCameraObject() { return activeCamera->GetParent(); }
			std::vector<Utilities::ID> GetAllObjects();
			void DestroyObject(Utilities::ID id);
			#ifdef StevEngine_PLAYER_DATA
			void ExportToFile();
			#endif
		private:
			Scene(std::string name);
			Scene(YAML::Node);
			bool active = false;
			void Activate();
			void Deactivate();
			Visuals::Camera* activeCamera;
			std::map<Utilities::ID, GameObject> gameObjects;
	};
}
