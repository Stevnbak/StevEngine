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
			static Engine* Instance;
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
		private:
			#ifdef StevEngine_SHOW_WINDOW
			void Draw();
			#endif
			void Update(double deltaTime);
		public:
			#ifdef StevEngine_SHOW_WINDOW
			SDL_Window* window;
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
			int targetFPS;
			double currentFPS;
			SDL_Event ev;
			#ifdef StevEngine_RENDERER_GL
			SDL_GLContext context;
			#endif
			#ifdef StevEngine_SHOW_WINDOW
			bool fullScreen;
			int WIDTH;
			int HEIGHT;
			#endif
			void (*mainUpdate)(double deltaTime);
	};
}
