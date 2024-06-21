#include "SceneManager.hpp"
#include "core/utilities/ID.hpp"

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
        scenes.insert(std::make_pair(name, Scene(name)));
        return &scenes.at(name);
    }
    Scene* SceneManager::CreateSceneFromFile(Resources::Resource file) {
        tinyxml2::XMLDocument doc;
		doc.Parse(file.GetStrData().c_str());
        tinyxml2::XMLElement* node = doc.FirstChildElement();
        Scene s = Scene(node);
        scenes.insert(std::make_pair(s.name, s));
        Scene* scene = &scenes.at(s.name);
        //Create objects
		tinyxml2::XMLElement* child = node->FirstChildElement();
		for(int i = 0; i < node->ChildElementCount(); i++) {
			std::string name = child->Name();
			if(name == "GameObject") {
				scene->CreateObjectFromXML(child);
			}
			child = child->NextSiblingElement();
		}
        //Set main camera
		scene->activeCamera = scene->GetObject(Utilities::ID(node->Attribute("camera")))->GetComponent<Camera>();
        //Return
        return scene;
    }
}