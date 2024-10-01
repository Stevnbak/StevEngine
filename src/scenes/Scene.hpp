#pragma once
#include "main/EventSystem.hpp"
#include "main/ResourceManager.hpp"
#include "main/multithreading/Lockable.hpp"
#include "scenes/GameObject.hpp"
#include "visuals/Camera.hpp"

#include <yaml-cpp/yaml.h>

#include <unordered_map>
#include <string>

namespace StevEngine {
	class Engine;
	class SceneManager;
	namespace Visuals { class Camera; };

	class Scene : Lockable {
		friend class Engine;
		friend class SceneManager;
		public:
			const std::string name;
		public:
			GameObject& CreateObject();
			GameObject& CreateObject(std::string name, Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1, 1, 1));
			GameObject& CreateObject(Resources::Resource file);
			GameObject& CreateObject(YAML::Node node);
			GameObject* GetObject(const Utilities::ID& id);
			#ifdef StevEngine_SHOW_WINDOW
			Visuals::Camera* GetCamera() const { return activeCamera; }
			GameObject* GetCameraObject() const { return activeCamera->GetParent(); }
			#endif
			std::vector<Utilities::ID> GetAllObjects() const;
			std::vector<Utilities::ID> GetAllParentObjects() const;
			void DestroyObject(const Utilities::ID& id);
			#ifdef StevEngine_PLAYER_DATA
			void ExportToFile() const;
			#endif
			EventManager& GetEvents() { return events; };
		private:
			EventManager events;
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
