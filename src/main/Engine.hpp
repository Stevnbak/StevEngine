#pragma once
#include "physics/System.hpp"
#include "main/ResourceManager.hpp"
#include "main/DataManager.hpp"
#include "audio/System.hpp"
#include "visuals/render/System.hpp"
#include "scenes/SceneManager.hpp"

#include <SDL.h>

namespace StevEngine {
	class Engine {
		public:
			static inline Engine* Instance = nullptr;
			Engine(
				const char * title = "Game"
				,int targetFPS = 60
				,void (*mainUpdate)(double deltaTime) = {}
				#ifdef StevEngine_SHOW_WINDOW
				,bool fullScreen = false
				,int WIDTH = 1080
				,int HEIGHT = 720
				#endif
			);
			int Start();
			#ifdef StevEngine_SHOW_WINDOW
			SDL_Window* window;
			void SetWindowSize(int width, int height);
			#endif
			Resources::System resources;
			#ifdef StevEngine_RENDERER_GL
			Render::System render;
			#endif
			#ifdef StevEngine_PHYSICS
			Physics::System physics;
			#endif
			#ifdef StevEngine_PLAYER_DATA
			GameData::System data;
			#endif
			#ifdef StevEngine_AUDIO
			Audio::System audio;
			#endif
			SceneManager scenes;
			double getFPS();
			const char * title;
		private:
			#ifdef StevEngine_SHOW_WINDOW
			void Draw();
			#endif
			void Update(double deltaTime);
			int targetFPS;
			double currentFPS;
			SDL_Event ev;
			#ifdef StevEngine_SHOW_WINDOW
			bool fullScreen;
			int WIDTH;
			int HEIGHT;
			#endif
			#ifdef StevEngine_RENDERER_GL
			SDL_GLContext context;
			#endif
			void (*mainUpdate)(double deltaTime);
	};
}
