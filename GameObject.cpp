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
		for (int i = 0; i < components.size(); i++) {
			Component* component = components[i];
			component->Start();
		}
	}
	void GameObject::Update(double deltaTime) {
		//Components
		///Log::Normal(std::format("Object ({}) update. Component amount: {}", id, components.size()), true);
		for (int i = 0; i < components.size(); i++) {
			Component* component = components[i];
			component->Update(deltaTime);
		}
	}
	void GameObject::Draw() {
		glPushMatrix();
		TransformView();
		//Components
		for (int i = 0; i < components.size(); i++) {
			Component* component = components[i];
			component->Draw();
		}
		glPopMatrix();
	}
	void GameObject::TransformView() {
		//Parent transform
		if (parent != nullptr) {
			parent->TransformView();
		}
		//Rotation
		glRotated(rotation.X, 1, 0, 0);
		glRotated(rotation.Y, 0, 1, 0);
		glRotated(rotation.Z, 0, 0, 1);
		//Position
		glTranslated(position.X, position.Y, position.Z);
		//Scale
		glScaled(scale.X, scale.Y, scale.Z);
	}

	//Constructors
	GameObject::GameObject() {
		id = GameObject::currentID++;
		name = "GameObject_" + id;
		Log::Normal(std::format("Creating game object with new id {}", id), true);
	}
	GameObject* GameObject::Create() {
		GameObject* object = new GameObject();
		GameObject::gameObjects.push_back(object);
		object->Start();
		return object;
	}
	GameObject* GameObject::Create(std::string name, Utilities::Vector3d position, Utilities::Vector3d rotation, Utilities::Vector3d scale) {
		GameObject* object = new GameObject();
		GameObject::gameObjects.push_back(object);
		object->name = name;
		object->position = position;
		object->rotation = rotation;
		object->scale = scale;
		object->Start();
		return object;
	}
	
	//Children functions
	int GameObject::AddChild(GameObject* gameObject) {
		children.push_back(gameObject);
		gameObject->parent = this;
		return children.size() - 1;
	}
	void GameObject::RemoveChild(int index) {
		GameObject* child = GetChild(index);
		child->parent = nullptr;
		children.erase(children.begin() + index);
	}
	GameObject* GameObject::GetChild(int index) {
		return children[index];
	}
	int GameObject::GetIndexFromName(std::string name) {
		for (int i = 0; i < children.size(); i++) {
			if (children[i]->name == name) return i;
		}
		return -1;
	}

	//Destroy
	void GameObject::Destroy() {
		Log::Normal(std::format("Destroying object with id {}", id));
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