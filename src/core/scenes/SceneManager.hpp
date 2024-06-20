#pragma once
#include "Scene.hpp"
#include "core/ResourceManager.hpp"
#include <string>


namespace StevEngine {
    class Engine;

    class SceneManager {
        friend class StevEngine::Engine;

        public:
            Scene* GetScene(std::string name) { return &scenes.at(name); }
            Scene* GetActiveScene() { return &scenes.at(active); }
            Scene* CreateSceneFromFile(Resources::Resource file);
            Scene* CreateScene(std::string name);
            void SetActiveScene(std::string name);
        private:
            SceneManager();
            void ActivateDefault();
            std::map<std::string, Scene> scenes;
            std::string active;
    };
}