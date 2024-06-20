#include "SceneManager.hpp"

namespace StevEngine {
    //Scene manager
    Scene* SceneManager::CreateSceneFromFile(Resources::Resource file) {
        scenes.insert(std::make_pair("name", Scene(file)));
        return &scenes.at("name");
    }
    Scene* SceneManager::CreateScene(std::string name) {
        scenes.insert(std::make_pair(name, Scene(name)));
        return &scenes.at(name);
    }
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
}