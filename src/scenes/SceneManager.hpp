#pragma once
#include "Scene.hpp"
#include "main/ResourceManager.hpp"

#include <cassert>
#include <string>

namespace StevEngine {
	class SceneManager {
		public:
			void Init();
			Scene& GetScene(std::string name);
			Scene& GetActiveScene();
			Scene& CreateSceneFromFile(Resources::Resource file);
			Scene& CreateScene(std::string name);
			void SetActiveScene(std::string name);
			void ActivateDefault();
		private:
			void Update(double deltaTime);
			#ifdef StevEngine_SHOW_WINDOW
			void Draw();
			#endif
			std::map<std::string, Scene> scenes;
			std::string active;
	};

	extern SceneManager sceneManager;
}
