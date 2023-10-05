#pragma once
#include "Utilities.hpp"
#include "Log.hpp"
#include <map>
#include <vector>
#include <any>
#include <type_traits>
#include "InputSystem.hpp"


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
					Log::Error("ComponentType must be derived from abstract class Component");
					return;
				}
				//Add to list
				Log::Normal("Adding component");
				ComponentType component = ComponentType();
				component.SetObject(this);
				components.push_back(component);
			}
			template <class ComponentType> void RemoveComponent() {
				//Check if component type is a Component
				if (!std::is_base_of_v<Component, ComponentType>) {
					Log::Error("Component must be derived from abstract class Component");
					return;
				}
				//Find component
				for (int i = 0; i < components.size(); i++) {
					if (std::any_cast<ComponentType>(&components[i])) {
						components.erase(components.begin() + i);
						return;
					}
				}
				Log::Error("No component of specified type found on object");
			}
			template <class ComponentType> ComponentType* GetComponent() {
				//Check if component type is a Component
				if (!std::is_base_of_v<Component, ComponentType>) {
					Log::Error("Component must be derived from abstract class Component");
					return NULL;
				}
				//Find component
				for (int i = 0; i < components.size(); i++) {
					if (std::any_cast<ComponentType>(&components[i])) {
						ComponentType* component = (ComponentType*)&components[i];
						return component;
					}
				}
				//Return null
				Log::Error("No component of specified type found on object");
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