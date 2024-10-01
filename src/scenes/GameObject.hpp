#pragma once
#include "main/EventSystem.hpp"
#include "main/multithreading/Lockable.hpp"
#include "utilities/ID.hpp"
#include "utilities/Vector3.hpp"
#include "utilities/Quaternion.hpp"
#include "main/Log.hpp"
#include "main/InputSystem.hpp"
#include "main/ResourceManager.hpp"
#include "scenes/Component.hpp"

#include <yaml-cpp/yaml.h>
#include <glm/mat4x4.hpp>

#include <map>
#include <vector>
#include <array>
#include <type_traits>

namespace StevEngine {
	//Child Events
	template<typename EventType>
	class ChildEvent : public Event {
		public:
			ChildEvent(const EventType& event) : event(event) {};
			const EventType& event;
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "Child" + EventType::GetStaticEventType(); }
	};

	//Object class
	class GameObject final : Lockable {
		friend class SceneManager;
		friend class Scene;
		//Basic properties
		public:
			std::string name;
			Utilities::ID Id() const { return id; }
		private:
			Utilities::ID id;
			std::string scene;
		//Transform
		public:
			Utilities::Vector3 GetPosition() const;
			Utilities::Quaternion GetRotation() const;
			Utilities::Vector3 GetScale() const;
			//Update properties
			void SetPosition(Utilities::Vector3 position, bool announce = true);
			void SetRotation(Utilities::Quaternion rotation, bool announce = true);
			void SetScale(Utilities::Vector3 scale, bool announce = true);
			void SetTransform(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale, bool announce = true);
			//Absolute properties
			Utilities::Vector3 GetWorldPosition() const;
			Utilities::Quaternion GetWorldRotation() const;
			Utilities::Vector3 GetWorldScale() const;
		private:
			Utilities::Vector3 position = Utilities::Vector3();
			Utilities::Quaternion rotation = Utilities::Quaternion();
			Utilities::Vector3 scale = Utilities::Vector3(1,1,1);
		//Main functions
		public:
			~GameObject();
			#ifdef StevEngine_PLAYER_DATA
			void ExportToFile(std::string name) const;
			#endif
			YAML::Node Export() const;
			void Import(YAML::Node node);
		private:
			void Start();
			void Deactivate();
			void Update(double deltaTime);
			#ifdef StevEngine_SHOW_WINDOW
			void Draw(glm::mat4x4 transform);
			#endif
			GameObject();
			GameObject(Utilities::ID id, std::string name, std::string scene);
		//Events
		public:
			template<typename EventType>
			Utilities::ID Subscribe(EventFunction<EventType> handler, bool allowFromChild = true) {
				if(allowFromChild) events.Subscribe<ChildEvent<EventType>>([handler] (ChildEvent<EventType> e) { handler(e.event); });
				return events.Subscribe<EventType>(handler);
			}
			template<typename EventType>
			void Publish(const EventType& event) {
				events.Publish(event);
				GameObject* parent = GetParent();
				if(parent != nullptr) parent->ChildPublish<EventType>(event, id);
			}
			void Unsubscribe(const std::string eventId, const Utilities::ID handlerId) { events.Unsubscribe(eventId, handlerId); }
		private:
			template<typename EventType>
			void ChildPublish(const EventType& event, Utilities::ID object) {
				events.Publish(ChildEvent<EventType>(event));
				GameObject* parent = GetParent();
				if(parent != nullptr) parent->ChildPublish<EventType>(event, object);
			}
			EventManager events;
		//Children functions
		public:
			int AddChild(const Utilities::ID& gameObjectID);
			void RemoveChild(int index);
			GameObject* GetChild(int index) const;
			int GetChildCount() const;
			GameObject* GetParent() const;
		private:
			Utilities::ID parent = Utilities::ID::empty;
			std::vector<Utilities::ID> children;
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
				if(!component) return nullptr;
				//If unique check uniqueness
				if (component->unique) {
					if (GetComponent<T>(false) != nullptr) {
						Log::Error(std::format("Object {} already has a component of type \"{}\", and this component requires to be unique", id.GetString(), typeid(T).name()), true);
						return nullptr;
					}
				}
				//Add to list
				component->SetObject(this, this->scene);
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

	//Transform Events
	class TransformUpdateEvent : public Event {
		public:
			TransformUpdateEvent(bool position = true, bool rotation = true, bool scale = true)
		 		: position(position), rotation(rotation), scale(scale) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "TransformUpdateEvent"; }
			bool position, rotation, scale;
	};
	#ifdef StevEngine_SHOW_WINDOW
		class DrawEvent : public Event {
			public:
				DrawEvent(glm::mat4x4 transform) : transform(transform) {}
				const std::string GetEventType() const override { return GetStaticEventType(); };
				static const std::string GetStaticEventType() {  return "DrawEvent"; }
				glm::mat4x4 transform;
		};
	#endif
	class DeactivateEvent : public Event {
		public:
			DeactivateEvent() {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "DeactivateEvent"; }
	};
}
