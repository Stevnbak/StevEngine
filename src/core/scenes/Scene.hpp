#pragma once
#include "core/ResourceManager.hpp"
#include <visuals/Camera.hpp>
#include "core/scenes/GameObject.hpp"

namespace StevEngine {
    class Engine;
    class SceneManager;
    
    class Scene {
        friend class Engine;
        friend class SceneManager;
        public:
            const std::string name;
        public:
			ID CreateObject();
			ID CreateObject(std::string name, Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1, 1, 1));
			ID CreateObjectFromFile(Resources::Resource file);
			GameObject* GetObject(ID id) { return &gameObjects.at(id); }
            GameObject* GetCamera() { return activeCamera->GetParent(); }
			std::vector<ID> GetAllObjects();
            void DestroyObject(ID id);
            void ExportToFile(Resources::Resource file);
		private:
            bool active;
            void Activate();
            void Deactivate();
            Scene(std::string name);
            Scene(Resources::Resource file);
			Camera* activeCamera;
			std::map<ID, GameObject> gameObjects;
			ID CreateObjectFromXML(tinyxml2::XMLElement* node);
    };
}