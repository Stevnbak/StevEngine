#include <iostream>
#include "GameObject.hpp"
#include <map>
#include <list>
#include <stdexcept>
#include <type_traits>

namespace StevEngine {
	//Static variables
	int GameObject::currentID = 0;
	std::vector<GameObject> GameObject::gameObjects;

	//Main functions
	void GameObject::Start() {
		//std::cout << "ID: " << id << std::endl;
	}
	void GameObject::Update(double deltaTime) {
		//If object is no longer in memory, destroy it
		if (id < 0) {
			std::cout << "GameObject no longer accessible in memory, being destroyed!" << std::endl;
			Destroy();
			return;
		}
		//Components
		std::cout << "Object update - " << id << " | Components: " << components.size() << std::endl;
		for (int i = 0; i < components.size(); i++) {
			Component* component = (Component*)(&components[i]);
			component->Update(deltaTime);
		}
	}
	void GameObject::Draw() {
		//Components
		for (int i = 0; i < components.size(); i++) {
			Component* component = (Component*)(&components[i]);
			component->Draw();
		}
	}
	//Constructors
	GameObject::GameObject() {
		id = GameObject::currentID++;
		std::cout << "Creating game object with new id " << id << std::endl;
	}
	GameObject* GameObject::Create() {
		GameObject object = GameObject();
		GameObject::gameObjects.push_back(object);
		object.Start();
		return &GameObject::gameObjects[GameObject::gameObjects.size() - 1];
	}
	//Destroy
	void GameObject::Destroy() {
		for (int i = 0; i < GameObject::gameObjects.size(); i++) {
			if (GameObject::gameObjects[i].id == id) {
				GameObject::gameObjects.erase(GameObject::gameObjects.begin() + i);
				break;
			}
		}
	}
}