#pragma once
#include "main/ResourceManager.hpp"
#include "main/GameObject.hpp"
#include "visuals/Camera.hpp"

#include <yaml-cpp/yaml.h>

#include <unordered_map>
#include <string>

namespace StevEngine {
	class Engine;
	class SceneManager;
	namespace Visuals { class Camera; };

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
			GameObject* GetObject(Utilities::ID id);
			#ifdef StevEngine_SHOW_WINDOW
			Visuals::Camera* GetCamera() { return activeCamera; }
			GameObject* GetCameraObject() { return activeCamera->GetParent(); }
			#endif
			std::vector<Utilities::ID> GetAllObjects();
			std::vector<Utilities::ID> GetAllParentObjects();
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
			#ifdef StevEngine_SHOW_WINDOW
			Visuals::Camera* activeCamera;
			#endif
			std::unordered_map<Utilities::ID, GameObject> gameObjects;
	};
}
