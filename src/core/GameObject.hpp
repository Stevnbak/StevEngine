#pragma once
#include <core/Utilities.hpp>
#include <core/Log.hpp>
#include <core/InputSystem.hpp>
#include <core/ResourceManager.hpp>
#include "Component.hpp"
#include <core/Engine.hpp>

#include <map>
#include <vector>
#include <any>
#include <type_traits>
#include <tinyxml2.h>

namespace StevEngine {
	using ID = uint16_t;

	class GameObject {
		friend class Engine;
		//Basic properties
		public:
			const std::string name;
			ID Id() { return id; }
		private:
			ID id;
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
			void ExportToFile(std::string path);
		private: 
			void Start();
			void Update(double deltaTime);
			void Draw();
			std::string Export();
			void TransformView();
			GameObject();
			GameObject(ID id, std::string name);
		//Children functions
		public:
			int AddChild(ID gameObjectID);
			void RemoveChild(int index);
			ID GetChild(int index);
			int GetChildCount();
			ID GetParent() { return parent; }
		private:
			ID parent = 0;
			std::vector<ID> children;
		//Static:
		public:
			static ID Create();
			static ID Create(std::string name, Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1, 1, 1));
			static ID CreateFromFile(Resources::Resource file);
			template<typename T> static ComponentFactory<T>* AddComponentFactory(std::string type) {
				ComponentFactory<T>* component = new ComponentFactory<T>();
				componentFactories.insert(std::make_pair(type, component));
				return component;
			};
			static GameObject* GetObject(ID id) { return &gameObjects.at(id); }
			static std::vector<ID> GetAllObjects();
		private:
			static inline std::map<std::string, FactoryBase*> componentFactories = std::map<std::string, FactoryBase*>();
			static std::map<ID, GameObject> gameObjects;
			static ID currentID;
			static ID CreateFromXML(tinyxml2::XMLElement* node);
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
				if (log) Log::Error(std::format("No component of type \"{}\" found on object {}", typeid(T).name(), id), true);
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
					std::vector<T*> childComp = gameObjects.at(children[i]).GetAllComponentsInChildren<T>();
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
						Log::Error(std::format("Object {} already has a component of type \"{}\", and this component requires to be unique", id, typeid(T).name()));
						return nullptr;
					}
				}
				//Add to list
				component->SetObject(this->id);
				component->Start();
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
				Log::Error(std::format("No component of type \"{}\" found on object {}", typeid(T).name(), id), true);
			}
	};
}