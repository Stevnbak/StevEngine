#pragma once
#include "main/EventSystem.hpp"
#include "utilities/ID.hpp"
#include "utilities/Vector3.hpp"
#include "utilities/Quaternion.hpp"
#include "utilities/Matrix4.hpp"
#include "main/Log.hpp"
#include "main/Component.hpp"

#include <memory>
#include <vector>
#include <type_traits>

namespace StevEngine {
	//Child Events
	/**
	 * @brief Wrapper for events from child objects
	 * @tparam EventType Type of wrapped event
	 */
	template<typename EventType>
	class ChildEvent : public Event {
		public:
			ChildEvent(const EventType& event) : event(event) {};
			const EventType& event;  ///< Original event
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "Child" + EventType::GetStaticEventType(); }
	};

	/**
	 * @brief Core game object class
	 *
	 * Base class for all entities in the game world. Handles transform hierarchy,
	 * component management, and event propagation.
	 */
	class GameObject final {
		friend class SceneManager;
		friend class Scene;
		//Basic properties
		public:
			/** @brief Object name/label */
			std::string name;

			/**
			 * @brief Get object's unique ID
			 * @return Unique identifier
			 */
			Utilities::ID Id() const { return id; }

		private:
			Utilities::ID id;	  	///< Unique identifier
			std::string scene;	  	///< Containing scene name
			bool isActive = false;	///< Is this gameobject currently active

		//Transform
		public:
			/**
			 * @brief Get local position
			 * @return Local position vector
			 */
			Utilities::Vector3 GetPosition() const;

			/**
			 * @brief Get local rotation
			 * @return Local rotation quaternion
			 */
			Utilities::Quaternion GetRotation() const;

			/**
			 * @brief Get local scale
			 * @return Local scale vector
			 */
			Utilities::Vector3 GetScale() const;

			//Update properties
			/**
			 * @brief Set local position
			 * @param position New position
			 * @param announce Whether to trigger transform event
			 */
			void SetPosition(Utilities::Vector3 position, bool announce = true);

			/**
			 * @brief Set local rotation
			 * @param rotation New rotation
			 * @param announce Whether to trigger transform event
			 */
			void SetRotation(Utilities::Quaternion rotation, bool announce = true);

			/**
			 * @brief Set local scale
			 * @param scale New scale
			 * @param announce Whether to trigger transform event
			 */
			void SetScale(Utilities::Vector3 scale, bool announce = true);

			/**
			 * @brief Set full local transform
			 * @param position New position
			 * @param rotation New rotation
			 * @param scale New scale
			 * @param announce Whether to trigger transform event
			 */
			void SetTransform(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale, bool announce = true);

			//World space properties
			/**
			 * @brief Get world position
			 * @return World space position
			 */
			Utilities::Vector3 GetWorldPosition();

			/**
			 * @brief Get world rotation
			 * @return World space rotation
			 */
			Utilities::Quaternion GetWorldRotation();

			/**
			 * @brief Get world scale
			 * @return World space scale
			 */
			Utilities::Vector3 GetWorldScale();

		private:
			Utilities::Vector3 position = Utilities::Vector3();		 ///< Local position
			Utilities::Quaternion rotation = Utilities::Quaternion();   ///< Local rotation
			Utilities::Vector3 scale = Utilities::Vector3(1,1,1);	  ///< Local scale

		//Main functions
		public:
			~GameObject();

			/**
			 * @brief Serialize object to a stream
			 * @param type Type of stream to export to
			 * @return Stream containing serialized object data
			 */
			Utilities::Stream Export(Utilities::StreamType type) const;

			/**
			 * @brief Load object from serialized data
			 * @param stream Stream containing serialized object data
			 */
			void Import(Utilities::Stream& stream);

		private:
			/**
			 * @brief Initialize object after creation
			 */
			void Start();

			/**
			 * @brief Clean up when object is deactivated
			 */
			void Deactivate();

			/**
			 * @brief Update object logic
			 * @param deltaTime Time since last update
			 */
			void Update(double deltaTime);

			#ifdef StevEngine_SHOW_WINDOW
			/**
			 * @brief Draw object visuals
			 * @param transform World transform matrix
			 */
			void Draw(Utilities::Matrix4 transform);
			#endif

		public:
			/**
			 * @brief Should never be called directly, use Scene::CreateObject
			 *
			 * Constructs a GameObject with the specified properties.
			 */
			GameObject(Utilities::ID id, std::string name, std::string scene);

		//Events
		public:
			/**
			 * @brief Subscribe to object events
			 * @tparam EventType Type of event to subscribe to
			 * @param handler Function to handle event
			 * @param allowFromChild Whether to receive events from children
			 * @return Subscription ID
			 */
			template<typename EventType>
			Utilities::ID Subscribe(EventFunction<EventType> handler, bool allowFromChild = true) {
				if(allowFromChild)
					events.Subscribe<ChildEvent<EventType>>([handler] (ChildEvent<EventType> e) { handler(e.event); });
				return events.Subscribe<EventType>(handler);
			}

			/**
			 * @brief Publish an event
			 * @tparam EventType Type of event to publish
			 * @param event Event to publish
			 */
			template<typename EventType>
			void Publish(const EventType& event) {
				events.Publish(event);
				GameObject* parent = GetParent();
				if(parent != nullptr) parent->ChildPublish<EventType>(event, id);
			}

			/**
			 * @brief Unsubscribe from events
			 * @param eventId Event type identifier
			 * @param handlerId Handler ID to unsubscribe
			 */
			void Unsubscribe(const std::string eventId, const Utilities::ID handlerId) { events.Unsubscribe(eventId, handlerId); }

		private:
			/**
			 * @brief Publish child event to parent
			 * @tparam EventType Type of event
			 * @param event Original event
			 * @param object Source object ID
			 */
			template<typename EventType>
			void ChildPublish(const EventType& event, Utilities::ID object) {
				events.Publish(ChildEvent<EventType>(event));
				GameObject* parent = GetParent();
				if(parent != nullptr) parent->ChildPublish<EventType>(event, object);
			}

			EventManager events;   ///< Object event manager
			std::vector<std::pair<Utilities::ID, std::string>> handlers;  ///< Event handler registrations

		//Children functions
		public:
			/**
			 * @brief Add child object
			 * @param gameObjectID ID of child object
			 * @return Index of new child
			 */
			int AddChild(const Utilities::ID& gameObjectID);

			/**
			 * @brief Remove child at index
			 * @param index Index of child to remove
			 */
			void RemoveChild(int index);

			/**
			 * @brief Get child object
			 * @param index Child index
			 * @return Pointer to child object
			 */
			GameObject* GetChild(int index) const;

			/**
			 * @brief Get number of children
			 * @return Child count
			 */
			uint GetChildCount() const;

			/**
			 * @brief Get parent object
			 * @return Pointer to parent object
			 */
			GameObject* GetParent() const;

			/**
			 * @brief Get containing Scene
			 * @return Pointer to containing Scene
			 */
			Scene* GetScene() const;

		private:
			/**
			 * @brief Set parent object
			 * @param id ID of parent object
			 */
			void SetParent(const Utilities::ID& id);

			Utilities::ID parent = Utilities::ID::empty;  ///< Parent object ID
			std::vector<Utilities::ID> children;		  ///< Child object IDs

		//Component functions
		private:
			std::vector<std::unique_ptr<Component>> components;  ///< Attached components

		public:
			/**
			 * @brief Get component of specified type
			 * @tparam T Component type to get
			 * @param log Whether to log if component not found
			 * @return Pointer to component or nullptr if not found
			 */
			template <class T>
			typename std::enable_if<std::is_base_of<Component,T>::value,T*>::type
			GetComponent(bool log = true) {
				//Find component
				for (int i = 0; i < components.size(); i++) {
					if (dynamic_cast<T*>(components[i].get())) {
						T* component = (T*)components[i].get();
						return component;
					}
				}
				//Return null
				if (log) Log::Error(std::format("No component of type \"{}\" found on object {}", typeid(T).name(), id.GetString()), true);
				return nullptr;
			}

			/**
			 * @brief Get all components of specified type
			 * @tparam T Component type to get
			 * @return Vector of matching components
			 */
			template <class T>
			typename std::enable_if<std::is_base_of<Component,T>::value, std::vector<T*> >::type
			GetAllComponents() {
				//Define vector
				std::vector<T*> foundComponents;
				//Find components
				for (int i = 0; i < components.size(); i++) {
					if (dynamic_cast<T*>(components[i].get())) {
						T* component = (T*)components[i].get();
						foundComponents.push_back(component);
					}
				}
				//Return null
				return foundComponents;
			}

			/**
			 * @brief Get all components of any type
			 * @return Vector of all components
			 */
			std::vector<Component*> GetAllComponents() const {
				std::vector<Component*> vec;
				for(auto& component : components) vec.push_back(component.get());
				return vec;
			}

			/**
			 * @brief Get components of type from this object and all children
			 * @tparam T Component type to get
			 * @return Vector of matching components
			 */
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
			/**
			 * @brief Add component to object
			 * @tparam T Component type to add
			 * @param component Component instance to add
			 * @return Pointer to added component or nullptr if failed
			 */
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
				components.emplace_back(component);
				if(isActive) component->Start();
				return component;
			}

			/**
			 * @brief Remove all components of specified type
			 * @tparam T Component type to remove
			 */
			template <class T>
			typename std::enable_if<std::is_base_of<Component,T>::value, void >::type
			RemoveAllComponents() {
				//Find component
				auto i = components.begin();
				while (i != components.end()) {
					if (dynamic_cast<T*>(*i)) {
						//Delete component from memory
						///delete *i;
						//Remove from list
						i = components.erase(i);
					}
					else {
						++i;
					}
				}
			}

			/**
			 * @brief Remove specific component instance
			 * @tparam T Component type to remove
			 * @param component Pointer to component to remove
			 */
			template <class T>
			typename std::enable_if<std::is_base_of<Component,T>::value, void >::type
			RemoveComponent(T* component) {
				//Find component
				for (int i = 0; i < components.size(); i++) {
					if (components[i].get() == component) {
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

	/**
	 * @brief Event for transform changes
	 */
	class TransformUpdateEvent : public Event {
		public:
			/**
			 * @brief Create transform update event
			 * @param position Whether position changed
			 * @param rotation Whether rotation changed
			 * @param scale Whether scale changed
			 */
			TransformUpdateEvent(bool position = true, bool rotation = true, bool scale = true)
			  : position(position), rotation(rotation), scale(scale) {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "TransformUpdateEvent"; }
			bool position, rotation, scale;  ///< Which transform components changed
	};

	#ifdef StevEngine_SHOW_WINDOW
		/**
		 * @brief Event for drawing
		 */
		class DrawEvent : public Event {
			public:
				/**
				 * @brief Create draw event
				 * @param transform World transform matrix
				 */
				DrawEvent(Utilities::Matrix4 transform) : transform(transform) {}
				const std::string GetEventType() const override { return GetStaticEventType(); };
				static const std::string GetStaticEventType() {  return "DrawEvent"; }
				Utilities::Matrix4 transform;  ///< World transform matrix
		};
	#endif

	/**
	 * @brief Event for object deactivation
	 */
	class DeactivateEvent : public Event {
		public:
			DeactivateEvent() {}
			const std::string GetEventType() const override { return GetStaticEventType(); };
			static const std::string GetStaticEventType() {  return "DeactivateEvent"; }
	};
}
