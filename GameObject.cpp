#include <iostream>
#include "GameObject.h"
#include <map>

namespace StevEngine {
	long GameObject::curID = 0;
	std::map<long, GameObject*> GameObject::objects = std::map<long, GameObject*>();
	void GameObject::Start() {

	}
	void GameObject::Update(double deltaTime) {
		std::cout << "Object update - " << id << std::endl;
	}
	void GameObject::Draw() {

	}
	GameObject::GameObject() {
		id = GameObject::curID++;
		GameObject::objects[id] = this;
		Start();
	}
	GameObject::GameObject(const GameObject& object) {
		id = GameObject::curID++;
		GameObject::objects[id] = this;
		position = object.position;
		rotation = object.rotation;
		scale = object.scale;
		Start();
	}
	void GameObject::Destroy() {
		GameObject::objects.erase(id);
	}
}