#pragma once
#include "main/ResourceManager.hpp"
#include "main/GameObject.hpp"
#include "utilities/Stream.hpp"
#include "visuals/Camera.hpp"

#include <unordered_map>
#include <string>

namespace StevEngine {
	class Engine;
	class SceneManager;
	namespace Visuals { class Camera; };

	/**
	 * @brief Container for game objects and scene state
	 *
	 * Manages a collection of GameObjects and their relationships.
	 * Handles object creation, destruction and lookup within the scene.
	 */
	class Scene {
		friend class Engine;
		friend class SceneManager;
		public:
			/** @brief Scene name/identifier */
			const std::string name;

		public:
			/**
			 * @brief Create empty GameObject
			 * @return ID of created object
			 */
			Utilities::ID CreateObject();

			/**
			 * @brief Create GameObject with transform
			 * @param name Object name
			 * @param position Initial position
			 * @param rotation Initial rotation
			 * @param scale Initial scale
			 * @return ID of created object
			 */
			Utilities::ID CreateObject(std::string name, Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1, 1, 1));

			/**
			 * @brief Create GameObject from resource file
			 * @param file Resource containing serialized object data
			 * @return ID of created object
			 */
			Utilities::ID CreateObject(Resources::Resource file, Utilities::StreamType type);

			/**
			 * @brief Create GameObject from serialized data
			 * @param stream Stream containing serialized object data
			 * @return ID of created object
			 */
			Utilities::ID CreateObject(Utilities::Stream& stream);

			/**
			 * @brief Get object by ID
			 * @param id Object identifier
			 * @return Pointer to object or nullptr if not found
			 */
			GameObject* GetObject(Utilities::ID id);

			#ifdef StevEngine_SHOW_WINDOW
			/**
			 * @brief Get main camera component
			 * @return Pointer to camera component
			 */
			Visuals::Camera* GetCamera() { return activeCamera; }

			/**
			 * @brief Get camera object
			 * @return Pointer to camera GameObject
			 */
			GameObject* GetCameraObject() { return activeCamera->GetParent(); }
			#endif

			/**
			 * @brief Get IDs of all objects in scene
			 * @return Vector of object IDs
			 */
			std::vector<Utilities::ID> GetAllObjects();

			/**
			 * @brief Get IDs of all root objects (no parent)
			 * @return Vector of root object IDs
			 */
			std::vector<Utilities::ID> GetAllParentObjects();

			/**
			 * @brief Destroy object and all children
			 * @param id ID of object to destroy
			 */
			void DestroyObject(Utilities::ID id);

			/**
			 * @brief Export entire scene
			 * @param type Type of stream to export to
			 * @return Stream containing serialized object data
			 */
			Utilities::Stream Export(Utilities::StreamType type);

		private:
			/**
			 * @brief Create new scene
			 * @param name Scene identifier
			 */
			Scene(std::string name);

			/**
			 * @brief Create scene from serialized data
			 * @param name Name of the scene
			 * @param stream Stream containing serialized object data
			 */
			Scene(std::string name, Utilities::Stream& stream);

			bool active = false;  ///< Whether scene is currently active

			/**
			 * @brief Activate scene and all objects
			 */
			void Activate();

			/**
			 * @brief Deactivate scene and all objects
			 */
			void Deactivate();

			#ifdef StevEngine_SHOW_WINDOW
			Visuals::Camera* activeCamera;  ///< Main scene camera
			#endif

			/** @brief Map of all objects in scene */
			std::unordered_map<Utilities::ID, GameObject> gameObjects;
	};
}
