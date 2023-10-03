#include <iostream>
#include "GameObject.h"
#include <map>

namespace StevEngine {
	int GameObject::curID = 0;
	std::map<int, GameObject*> GameObject::objects = std::map<int, GameObject*>();
	void GameObject::Update(double deltaTime) {
		std::cout << "Update - " << id << std::endl;
	}
	void GameObject::Draw() {

	}

	GameObject::GameObject() {
		id = GameObject::curID++;
		GameObject::objects[id] = this;
	}
	GameObject::GameObject(const GameObject& object) {
		id = GameObject::curID++;
		GameObject::objects[id] = this;
		position = object.position;
		rotation = object.rotation;
		scale = object.scale;
	}
	//Destructor
	void GameObject::Destroy() {
		GameObject::objects.erase(id);
	}
}