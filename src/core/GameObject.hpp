#pragma once
#include <core/Utilities.hpp>
#include <core/Log.hpp>
#include <core/InputSystem.hpp>
#include <core/ResourceManager.hpp>

#include <map>
#include <vector>
#include <any>
#include <type_traits>
#include <tinyxml2.h>

namespace StevEngine {
	class GameObject;
	class Component;

	class FactoryBase {
		public:
			virtual Component* create(tinyxml2::XMLElement* node) const = 0;
	};
	template <class T> class ComponentFactory : public FactoryBase {
		public:
			Component* create(tinyxml2::XMLElement* node) const {
				return new T(node);
			}
	};
	
	class Component {
		public:
			virtual void Start() = 0;
			virtual void Update(double deltaTime) = 0;
			virtual void Draw() = 0;
			virtual void Export(tinyxml2::XMLElement* element) = 0;
			std::string Export();
			void Destroy();
			void SetObject(GameObject* object);
			static const bool unique = false;
			GameObject* gameObject;
			Component(std::string type);
			Component(tinyxml2::XMLElement* node);
			virtual void TransformUpdate(bool position, bool rotation, bool scale) {};
		private:
			std::string type;
			static const FactoryBase* factory;
	};

	class GameObject {
		//Basic properties
		public:
			std::string name;
		private:
			int id;
			std::vector<Component*> components;
			std::vector<GameObject*> children;
		//Transform
		private:
			Utilities::Vector3 position = Utilities::Vector3();
			Utilities::Quaternion rotation = Utilities::Quaternion();
			Utilities::Vector3 scale = Utilities::Vector3(1,1,1);
		public:
			Utilities::Vector3 GetPosition();
			Utilities::Quaternion GetRotation();
			Utilities::Vector3 GetScale();
			//Update properties
			void SetPosition(Utilities::Vector3 position, bool announce = true);
			void SetRotation(Utilities::Quaternion rotation, bool announce = true);
			void SetScale(Utilities::Vector3 scale, bool announce = true);
			void SetTransform(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale, bool announce = true);
			void TransformUpdate(bool position, bool rotation, bool scale);
			//Absolute properties
			Utilities::Vector3 GetWorldPosition();
			Utilities::Quaternion GetWorldRotation();
			Utilities::Vector3 GetWorldScale();
		//Main functions
		public:
			void Start();
			void Update(double deltaTime);
			void Draw();
			void TransformView();
			void Destroy();
			//Children functions
			GameObject* parent = nullptr;
			int AddChild(GameObject* gameObject);
			void RemoveChild(int index);
			GameObject* GetChild(int index);
			int GetChildCount();
			int GetIndexFromName(std::string name);
			//Export functions
			std::string Export();
			void ExportToFile(std::string path);
		private: 
			GameObject();
		//Static:
		public:
			static GameObject* Create();
			static GameObject* Create(std::string name, Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1, 1, 1));
			static GameObject* CreateFromFile(Resources::Resource file);
			static GameObject* CreateFromXML(tinyxml2::XMLElement* node);
			static std::vector<GameObject*> GetGameObjects() { return gameObjects; }
			template<typename T> static ComponentFactory<T>* AddComponentFactory(std::string type) {
				ComponentFactory<T>* component = new ComponentFactory<T>();
				componentFactories.insert(std::make_pair(type, component));
				return component;
			};
		private:
			static inline std::map<std::string, FactoryBase*> componentFactories = std::map<std::string, FactoryBase*>();
			static std::vector<GameObject*> gameObjects;
			static int currentID;
		//Component functions
		public:
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
				return foundComponents;
			}
			template <class ComponentType> std::vector<ComponentType*> GetAllComponentsInChildren() {
				//Define vector
				std::vector<ComponentType*> allComponents;
				//Check if component type is a Component
				if (!std::is_base_of_v<Component, ComponentType>) {
					Log::Error("Component must be derived from abstract class Component", true);
					return allComponents;
				}
				//Find components in this object:
				std::vector<ComponentType*> current = GetAllComponents<ComponentType>();
				allComponents.insert(allComponents.end(), current.begin(), current.end());
				//Find components in each child object
				for (int i = 0; i < children.size(); i++) {
					GameObject* child = children[i];
					std::vector<ComponentType*> childComp = child->GetAllComponentsInChildren<ComponentType>();
					allComponents.insert(allComponents.end(), childComp.begin(), childComp.end());
				}
				//Return components
				return allComponents;
			}
			template <class ComponentType> ComponentType* AddComponent(ComponentType* component) {
				//Check if component type is a Component
				if (!std::is_base_of_v<Component, ComponentType>) {
					Log::Error("ComponentType must be derived from abstract class Component", true);
					return nullptr;
				}
				//If unique check uniqueness
				if (component->unique) {
					if (GetComponent<ComponentType>(false) != nullptr) {
						Log::Error(std::format("Object {} already has a component of type \"{}\", and this component requires to be unique", id, typeid(ComponentType).name()));
						return nullptr;
					}
				}
				//Add to list
				component->SetObject(this);
				component->Start();
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
				std::vector<Component*>::iterator i = components.begin();
				while (i != components.end()) {
					if (dynamic_cast<ComponentType*>(*i)) {
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
	};
}