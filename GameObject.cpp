#include "Log.hpp"
#include "GameObject.hpp"
#include <format>
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
		
	}
	void GameObject::Update(double deltaTime) {
		//Components
		///Log::Normal(std::format("Object ({}) update. Component amount: {}", id, components.size()));
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
		Log::Normal("Creating game object with new id " + id);
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

	//Component
	void Component::SetObject(GameObject* object) {
		gameObject = object;
	}
}