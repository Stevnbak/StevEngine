#pragma once
#include <utilities/Vector3.hpp>
#include <utilities/Quaternion.hpp>
#include <main/Log.hpp>
#include <main/InputSystem.hpp>
#include <main/ResourceManager.hpp>
#include <scenes/Component.hpp>

#include <map>
#include <vector>
#include <array>
#include <type_traits>
#include <tinyxml2.h>

namespace StevEngine {
	class GameObject {
		friend class Engine;
		friend class Scene;
		//Basic properties
		public:
			const std::string name;
			Utilities::ID Id() { return id; }
		private:
			Utilities::ID id;
			std::string scene;
		//Transform
		public:
			Utilities::Vector3 GetPosition();
			Utilities::Quaternion GetRotation();
			Utilities::Vector3 GetScale();
			//Update properties
			void SetPosition(Utilities::Vector3 position, bool announce = true);
			void SetRotation(Utilities::Quaternion rotation, bool announce = true);
			void SetScale(Utilities::Vector3 scale, bool announce = true);
			void SetTransform(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale, bool announce = true);
			//Absolute properties
			Utilities::Vector3 GetWorldPosition();
			Utilities::Quaternion GetWorldRotation();
			Utilities::Vector3 GetWorldScale();
		private:
			Utilities::Vector3 position = Utilities::Vector3();
			Utilities::Quaternion rotation = Utilities::Quaternion();
			Utilities::Vector3 scale = Utilities::Vector3(1,1,1);
			void TransformUpdate(bool position, bool rotation, bool scale);
		//Main functions
		public:
			~GameObject();
			void ExportToFile(std::string name);
		private: 
			void Start();
			void Deactivate();
			void Update(double deltaTime);
			void Draw();
			std::string Export();
			void TransformView();
			GameObject();
			GameObject(Utilities::ID id, std::string name, std::string scene);
		//Children functions
		public:
			int AddChild(Utilities::ID gameObjectID);
			void RemoveChild(int index);
			GameObject* GetChild(int index);
			int GetChildCount();
			GameObject* GetParent();
		private:
			Utilities::ID parent = Utilities::ID::empty;
			std::vector<Utilities::ID> children;
		//Static:
		public:
			template<typename T> static ComponentFactory<T>* AddComponentFactory(std::string type) {
				ComponentFactory<T>* component = new ComponentFactory<T>();
				componentFactories.insert(std::make_pair(type, component));
				return component;
			};
		private:
			static inline std::map<std::string, FactoryBase*> componentFactories = std::map<std::string, FactoryBase*>();
		//Component functions
		private:
			std::vector<Component*> components;
		public:
			template <class T>
			typename std::enable_if<std::is_base_of<Component,T>::value,T*>::type
			GetComponent(bool log = true) {
				//Find component
				for (int i = 0; i < components.size(); i++) {
					if (dynamic_cast<T*>(components[i])) {
						T* component = (T*)components[i];
						return component;
					}
				}
				//Return null
				if (log) Log::Error(std::format("No component of type \"{}\" found on object {}", typeid(T).name(), id.GetString()), true);
				return nullptr;
			}
			template <class T>
			typename std::enable_if<std::is_base_of<Component,T>::value, std::vector<T*> >::type
			GetAllComponents() {
				//Define vector
				std::vector<T*> foundComponents;
				//Find components
				for (int i = 0; i < components.size(); i++) {
					if (dynamic_cast<T*>(components[i])) {
						T* component = (T*)components[i];
						foundComponents.push_back(component);
					}
				}
				//Return null
				return foundComponents;
			}
			template <class T>
			typename std::enable_if<std::is_base_of<Component,T>::value, std::vector<T*> >::type
			GetAllComponentsInChildren() {
				//Define vector
				std::vector<T*> allComponents;
				//Find components in this object:
				std::vector<T*> current = GetAllComponents<T>();
				allComponents.insert(allComponents.end(), current.begin(), current.end());
				//Find components in each child object
				for (int i = 0; i < children.size(); i++) {
					std::vector<T*> childComp = GetChild(i)->GetAllComponentsInChildren<T>();
					allComponents.insert(allComponents.end(), childComp.begin(), childComp.end());
				}
				//Return components
				return allComponents;
			}
			template <class T>
			typename std::enable_if<std::is_base_of<Component,T>::value, T* >::type
			AddComponent(T* component) {
				//If unique check uniqueness
				if (component->unique) {
					if (GetComponent<T>(false) != nullptr) {
						Log::Error(std::format("Object {} already has a component of type \"{}\", and this component requires to be unique", id.GetString(), typeid(T).name()), true);
						return nullptr;
					}
				}
				//Add to list
				component->SetObject(this->id, this->scene);
				components.push_back(component);
				return component;
			}
			template <class T>
			typename std::enable_if<std::is_base_of<Component,T>::value, void >::type
			RemoveAllComponents() {
				//Find component
				std::vector<Component*>::iterator i = components.begin();
				while (i != components.end()) {
					if (dynamic_cast<T*>(*i)) {
						//Delete component from memory
						delete *i;
						//Remove from list
						i = components.erase(i);
					}
					else {
						++i;
					}
				}
			}
			template <class T>
			typename std::enable_if<std::is_base_of<Component,T>::value, void >::type
			RemoveComponent(T* component) {
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
				Log::Error(std::format("No component of type \"{}\" found on object {}", typeid(T).name(), id.GetString()), true);
			}
	};
}