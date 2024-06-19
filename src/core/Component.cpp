#include "Component.hpp"

namespace StevEngine {
    Component::Component(std::string type) : type(type) {}
    Component::Component(tinyxml2::XMLElement* element) : Component(element->Attribute("type")) {}
    void Component::SetObject(ID object) {
        gameObject = object;
    }
    Component::~Component() {
        
    }
}