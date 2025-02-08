#pragma once
#include "Scene.hpp"
#include "main/ResourceManager.hpp"

#include <string>

namespace StevEngine {
	/**
	 * @brief Manages multiple scenes and scene transitions
	 *
	 * Handles creation, loading and switching between scenes.
	 * Maintains references to all loaded scenes and the active scene.
	 */
	class SceneManager {
		public:
			/**
			 * @brief Initialize scene manager
			 */
			void Init();

			/**
			 * @brief Get scene by name
			 * @param name Scene identifier
			 * @return Pointer to scene
			 */
			Scene* GetScene(std::string name) { return &scenes.at(name); }

			/**
			 * @brief Get currently active scene
			 * @return Pointer to active scene
			 */
			Scene* GetActiveScene() { return &scenes.at(active); }

			/**
			 * @brief Create scene from serialized data
			 * @param file Resource containing scene data
			 * @return Pointer to created scene
			 */
			Scene* CreateSceneFromFile(Resources::Resource file);

			/**
			 * @brief Create empty scene
			 * @param name Scene identifier
			 * @return Pointer to created scene
			 */
			Scene* CreateScene(std::string name);

			/**
			 * @brief Switch to different scene
			 * @param name Name of scene to activate
			 */
			void SetActiveScene(std::string name);

			/**
			 * @brief Ensure a scene is active
			 *
			 * Creates default scene if none exist.
			 * Sets first scene as active if none active.
			 */
			void ActivateDefault();

		private:
			/**
			 * @brief Update active scene
			 * @param deltaTime Time since last update
			 */
			void Update(double deltaTime);

			#ifdef StevEngine_SHOW_WINDOW
			/**
			 * @brief Draw active scene
			 */
			void Draw();
			#endif

			std::map<std::string, Scene> scenes;  ///< Map of all loaded scenes
			std::string active;				   ///< Name of active scene
	};

	/** Global scene manager instance */
	extern SceneManager sceneManager;
}
