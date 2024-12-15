#pragma once
#include "main/EventSystem.hpp"
#include "main/Log.hpp"
#include "utilities/ID.hpp"
#include "utilities/Matrix4.hpp"

#include <yaml-cpp/yaml.h>

#include <sys/types.h>
#include <functional>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

namespace StevEngine {
	class GameObject;
	class Scene;

	/**
	 * @brief Base class for all game object components
	 *
	 * Components provide behavior and functionality to GameObjects.
	 * They can respond to events, be serialized, and manage resources.
	 */
	class Component {
		friend class GameObject;
		//Properties
		private:
			/** @brief ID of parent GameObject */
			Utilities::ID gameObject = Utilities::ID::empty;

			/** @brief Name of containing scene */
			std::string scene;

			/** @brief Whether only one instance can exist per GameObject */
			static const bool unique = false;

		protected:
			/** @brief Type identifier for this component */
			std::string type;

		//Functions
		public:
			/**
			 * @brief Create new component
			 * @param type Type identifier string
			 */
			Component(std::string type);

			/**
			 * @brief Clean up component resources
			 */
			virtual ~Component();

			/**
			 * @brief Get parent GameObject
			 * @return Pointer to parent GameObject
			 */
			GameObject* GetParent() const;

			/**
			 * @brief Get containing Scene
			 * @return Pointer to containing Scene
			 */
			Scene* GetScene() const;

			/**
			 * @brief Get component type
			 * @return Type identifier string
			 */
			std::string GetType() const { return type; }

			/**
			 * @brief Serialize component to YAML
			 * @param node Node to serialize into
			 * @return Updated node
			 */
			virtual YAML::Node Export(YAML::Node node) const;

			/**
			 * @brief Serialize component to new YAML node
			 * @return New node containing serialized data
			 */
			YAML::Node Export() const;

			/**
			 * @brief Create component from serialized data
			 * @param node YAML node containing component data
			 */
			Component(YAML::Node node);

		private:
			/**
			 * @brief Initialize component after creation
			 */
			virtual void Start() {};

			/**
			 * @brief Clean up when component is deactivated
			 */
			virtual void Deactivate() {};

			/**
			 * @brief Update component logic
			 * @param deltaTime Time since last update
			 */
			virtual void Update(double deltaTime) {};

			#ifdef StevEngine_SHOW_WINDOW
			/**
			 * @brief Draw component visuals
			 * @param transform World transform matrix
			 */
			virtual void Draw(const Utilities::Matrix4& transform) {};
			#endif

			/**
			 * @brief Set parent GameObject and scene
			 * @param object Parent GameObject
			 * @param scene Containing scene name
			 */
			void SetObject(GameObject* object, std::string scene);

		//Events
		protected:
			/** @brief Event handler registrations */
			std::vector<std::pair<Utilities::ID, std::string>> handlers;
	};

	/**
	 * @brief Factory system for creating components from serialized data
	 *
	 * Manages component type registration and instantiation from YAML data.
	 * Allows components to be created dynamically at runtime.
	 */
	class CreateComponents {
		/** @brief Map of registered component factory functions */
		static inline std::unordered_map<std::string, std::function<Component*(YAML::Node node)>> factories = std::unordered_map<std::string, std::function<Component*(YAML::Node node)>>();

		public:
			/**
			 * @brief Create component from serialized data
			 * @param node YAML node containing component data
			 * @return Pointer to created component
			 */
			static Component* Create(YAML::Node node);

			/**
			 * @brief Register a component type for creation
			 * @tparam T Component class type
			 * @param type String identifier for component type
			 * @return true if registration succeeded, false if type already registered
			 */
			template <class T> static bool RegisterComponentType(std::string type) {
				if(factories.contains(type)) return false;
				factories.insert({type, [](YAML::Node node) -> Component*  {
					return (Component*) (new T(node));
				}});
				return true;
			}
	};
}
