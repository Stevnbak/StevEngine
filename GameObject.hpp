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
			template <class ComponentType> ComponentType* GetComponent(bool log = true) {
				//Check if component type is a Component
				if (!std::is_base_of_v<Component, ComponentType>) {
					if(log) Log::Error("Component must be derived from abstract class Component", true);
					return nullptr;
				}
				//Find component
				for (int i = 0; i < components.size(); i++) {
					if (dynamic_cast<ComponentType*>(components[i])) {
						ComponentType* component = (ComponentType*)components[i];
						return component;
					}
				}
				//Return null
				if (log) Log::Error(std::format("No component of type \"{}\" found on object {}", typeid(ComponentType).name(), id), true);
				return nullptr;
			}
			template <class ComponentType> std::vector<ComponentType*> GetAllComponents() {
				//Define vector
				std::vector<ComponentType*> foundComponents;
				//Check if component type is a Component
				if (!std::is_base_of_v<Component, ComponentType>) {
					Log::Error("Component must be derived from abstract class Component", true);
					return foundComponents;
				}
				//Find components
				for (int i = 0; i < components.size(); i++) {
					if (dynamic_cast<ComponentType*>(components[i])) {
						ComponentType* component = (ComponentType*)components[i];
						foundComponents.push_back(component);
					}
				}
				//Return null
				Log::Error(std::format("No component of type \"{}\" found on object {}", typeid(ComponentType).name(), id), true);
				return foundComponents;
			}
			template <class ComponentType> ComponentType* AddComponent() {
				//Check if component type is a Component
				if (!std::is_base_of_v<Component, ComponentType>) {
					Log::Error("ComponentType must be derived from abstract class Component", true);
					return nullptr;
				}
				//Create object
				ComponentType* component = new ComponentType();
				//If unique check uniqueness
				if (component->unique) {
					if (GetComponent<ComponentType>(false) != nullptr) {
						Log::Error(std::format("Object {} already has a component of type \"{}\", and this component requires to be unique", id, typeid(ComponentType).name()));
						return nullptr;
					}
				}
				//Add to list
				component->SetObject(this);
				components.push_back(component);
				return component;
			}
			template <class ComponentType> void RemoveAllComponents() {
				//Check if component type is a Component
				if (!std::is_base_of_v<Component, ComponentType>) {
					Log::Error("Component must be derived from abstract class Component", true);
					return;
				}
				//Find component
				for (int i = 0; i < components.size(); i++) {
					if (dynamic_cast<ComponentType*>(components[i])) {
						//Remove from list
						components.erase(components.begin() + i);
						//Delete component from memory
						delete components[i];
					}
				}
			}
			template <class ComponentType> void RemoveComponent(ComponentType* component) {
				//Check if component type is a Component
				if (!std::is_base_of_v<Component, ComponentType>) {
					Log::Error("Component must be derived from abstract class Component", true);
					return;
				}
				//Find component
				for (int i = 0; i < components.size(); i++) {
					if (components[i] == component) {
						//Remove from list
						components.erase(components.begin() + i);
						//Delete component from memory
						delete component;
						return;
					}
				}
				Log::Error(std::format("No component of type \"{}\" found on object {}", typeid(ComponentType).name(), id), true);
			}
			//Creat GameObject
			static GameObject* Create();
			//Destroy
			void Destroy();
			//Get objects
			static std::vector<GameObject*> GetGameObjects() {
				return gameObjects;
			}
			//Default Constructor
			GameObject();
		private: 
			int id;
			std::vector<Component*> components;
			static std::vector<GameObject*> gameObjects;
			static int currentID;
	};

	class Component {
		public:
			virtual void Update(double deltaTime) = 0;
			virtual void Draw() = 0;
			void SetObject(GameObject* object);
			bool unique = true;
		private:
			GameObject* gameObject;
	};
}