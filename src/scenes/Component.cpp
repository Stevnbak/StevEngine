#include "Component.hpp"
#include "main/Engine.hpp"

using namespace StevEngine::Utilities;

namespace StevEngine {
    Component::Component(std::string type) : type(type) {}
    Component::Component(tinyxml2::XMLElement* element) : Component(element->Attribute("type")) {}
    void Component::SetObject(ID object, std::string scene) {
        gameObject = object;
		this->scene = scene;
    }
	GameObject* Component::GetParent() {
		return GetScene()->GetObject(gameObject);
	}
	Scene* Component::GetScene() {
		return Engine::Instance->scenes.GetScene(scene);
	}
    Component::~Component() {
        
    }
    //Export component
	std::string Component::Export() {
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLElement* main = doc.NewElement("Component");
		doc.InsertFirstChild(main);
		//Add basic info
		main->SetAttribute("type", type.c_str());

		//Add component specific info
		this->Export(main);
		
		//Return xml as string
		tinyxml2::XMLPrinter printer;
		doc.Print( &printer );
		return printer.CStr();
	}
}