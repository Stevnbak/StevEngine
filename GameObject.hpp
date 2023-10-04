#pragma once
#include "Utilities.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <any>
#include <type_traits>


namespace StevEngine {
	class Component;
	class GameObject {
		public:
			//Basic properties
			Utilities::Vector3d position = Utilities::Vector3d();
			Utilities::Vector3d rotation = Utilities::Vector3d();
			Utilities::Vector3d scale = Utilities::Vector3d(1,1,1);
			//Main functions
			void Start();
			void Update(double deltaTime);
			void Draw();
			//Components
			template <class ComponentType> void AddComponent() {
				//Check if component type is a Component
				if (!std::is_base_of_v<Component, ComponentType>) {
					std::cout << "[ERROR] - ComponentType must be derived from abstract class Component" << std::endl;
					return;
				}
				//Add to list
				std::cout << "Adding component" << std::endl;
				ComponentType component = ComponentType();
				component.SetObject(this);
				components.push_back(component);
			}
			template <class ComponentType> ComponentType* GetComponent() {
				//Check if component type is a Component
				if (!std::is_base_of_v<Component, ComponentType>) {
					std::cout << "[ERROR] - ComponentType must be derived from abstract class Component" << std::endl;
					return NULL;
				}
				//Find component
				for (int i = 0; i < components.size(); i++) {
					try {
						ComponentType* component = (ComponentType*)&components[i];
						return component;
					}
					catch (std::exception e) {

					}
				}
				//Return null
				return NULL;
			}
			//Creat GameObject
			static GameObject* Create();
			//Destroy
			void Destroy();
			//Get objects
			static std::vector<GameObject*> GetGameObjects() {
				std::vector<GameObject*> objects;
				for (int i = 0; i < gameObjects.size(); i++) {
					objects.push_back(&gameObjects[i]);
				}
				return objects;
			}
			//Default Constructor
			GameObject();
		private: 
			int id;
			std::vector<std::any> components;
			static std::vector<GameObject> gameObjects;
			static int currentID;
	};

	class Component {
		public:
			virtual void Update(double deltaTime) = 0;
			virtual void Draw() = 0;
			void SetObject(GameObject* object);
		private:
			GameObject* gameObject;
	};
}