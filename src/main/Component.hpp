#pragma once
#include "utilities/ID.hpp"
#include "utilities/Matrix4.hpp"
#include "utilities/Stream.hpp"

#include <sys/types.h>
#include <functional>
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
		friend class StevEngine::GameObject;
		//Properties
		private:
			/** @brief ID of parent GameObject */
			Utilities::ID gameObject = Utilities::ID::empty;

			/** @brief Name of containing scene */
			std::string scene;

			/** @brief Whether only one instance can exist per GameObject */
			static const bool unique = false;

		//Functions
		public:
			/**
			 * @brief Create new component
			 */
			Component();

			/**
			 * @brief Clean up component resources
			 */
			virtual ~Component();

			/**
			 * @brief Get parent GameObject
			 * @return Reference to parent GameObject
			 */
			GameObject& GetParent() const;

			/**
			 * @brief Get containing Scene
			 * @return Reference to containing Scene
			 */
			Scene& GetScene() const;

			/**
			 * @brief Get component type
			 * @return Type identifier string
			 */
			virtual std::string GetType() const = 0;

			/**
			 * @brief Serialize component to a stream
			 * @param type Type of stream to export to
			 * @return Serialized stream
			 */
			virtual Utilities::Stream Export(Utilities::StreamType stream) const = 0;

			/**
			 * @brief Initialize component after creation
			 */
			virtual void Start() {};

		private:
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
			void SetObject(GameObject& object, std::string scene);

		//Events
		protected:
			/** @brief Event handler registrations */
			std::vector<std::pair<Utilities::ID, std::string>> handlers;
	};

	/**
	 * @brief Factory system for creating components from serialized data
	 *
	 * Manages component type registration and instantiation from serialization data.
	 * Allows components to be created dynamically at runtime.
	 */
	class CreateComponents {
		/** @brief Map of registered component factory functions */
		static inline std::unordered_map<std::string, std::function<Component*(Utilities::Stream& stream)>> factories = std::unordered_map<std::string, std::function<Component*(Utilities::Stream& stream)>>();

		public:
			/**
			 * @brief Create component from serialized data
			 * @param type type string of the component to create
			 * @param stream text serializable stream containing component data
			 * @return Pointer to created component
			 */
			static Component* Create(const std::string& type, Utilities::Stream& stream);

			/**
			 * @brief Register a component type for creation
			 * @tparam T Component class type
			 * @param type String identifier for component type
			 * @return true if registration succeeded, false if type already registered
			 */
			template <class T> static bool RegisterComponentType(std::string type) {
				if(factories.contains(type)) return false;
				/*factories.insert({type, [](Utilities::Stream& stream) {
					return (Component*) (new T(stream));
				}});*/
				factories.emplace(type, [](Utilities::Stream& stream) {
					return (Component*) new T(stream);
				});
				return true;
			}
	};
}
