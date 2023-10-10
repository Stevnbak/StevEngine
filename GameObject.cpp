#include "Log.hpp"
#include "GameObject.hpp"
#include <format>
#include <map>
#include <list>
#include <stdexcept>
#include <type_traits>
#include <SDL2/include/SDL_opengl.h>
#include <iostream>

namespace StevEngine {
	//Static variables
	int GameObject::currentID = 0;
	std::vector<GameObject*> GameObject::gameObjects;

	//Main functions
	void GameObject::Start() {
		
	}
	void GameObject::Update(double deltaTime) {
		//Components
		///Log::Normal(std::format("Object ({}) update. Component amount: {}", id, components.size()), true);
		for (int i = 0; i < components.size(); i++) {
			Component* component = components[i]; //(Component&)(components[i]);
			component->Update(deltaTime);
		}
	}
	void GameObject::Draw() {
		glPushMatrix();
		//Rotation
		glRotated(rotation.X, 1, 0, 0);
		glRotated(rotation.Y, 0, 1, 0);
		glRotated(rotation.Z, 0, 0, 1);
		//Position
		glTranslated(position.X, position.Y, position.Z);
		//Scale
		glScaled(scale.X, scale.Y, scale.Z);
		//Components
		for (int i = 0; i < components.size(); i++) {
			Component* component = components[i]; //(Component*)(&components[i]);
			component->Draw();
		}
		glPopMatrix();
	}
	//Constructors
	GameObject::GameObject() {
		id = GameObject::currentID++;
		Log::Normal("Creating game object with new id " + id, true);
	}
	GameObject* GameObject::Create() {
		GameObject* object = new GameObject();
		GameObject::gameObjects.push_back(object);
		object->Start();
		return object;
	}
	//Destroy
	void GameObject::Destroy() {
		Log::Normal("Destroying object with id " + id);
		//Remove from gameobject list
		for (int i = 0; i < GameObject::gameObjects.size(); i++) {
			if (GameObject::gameObjects[i]->id == id) {
				GameObject::gameObjects.erase(GameObject::gameObjects.begin() + i);
				break;
			}
		}
		//Clear component memory
		for (int i = 0; i < components.size(); i++)
		{
			delete components[i];
		}
		//Clear memory
		delete this;
	}

	//Component
	void Component::SetObject(GameObject* object) {
		gameObject = object;
	}
}