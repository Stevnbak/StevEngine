#pragma once
#include "physics/System.hpp"
#include "main/ResourceManager.hpp"
#include "main/DataManager.hpp"
#include "main/Settings.hpp"
#include "audio/System.hpp"
#include "visuals/render/System.hpp"
#include "scenes/SceneManager.hpp"

#include <SDL.h>

namespace StevEngine {
	struct GameSettings {
		#ifdef StevEngine_SHOW_WINDOW
		bool vsync = false;
		bool fullscreen = false;
		int WIDTH = 800;
		int HEIGHT = 600;
		#endif
		int targetFPS = 60;
	};
	class Engine {
		public:
			static inline Engine* Instance = nullptr;
			Engine(const char * title = "Game", GameSettings gameSettings = GameSettings(), void (*mainUpdate)(double deltaTime) = nullptr);
			int Start();
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
			GameData data;
			Settings settings;
			#endif
			#ifdef StevEngine_AUDIO
			Audio::System audio;
			#endif
			SceneManager scenes;
			double getFPS();
			//Engine settings
			const char * title;
			GameSettings GetGameSettings() { return gameSettings; }
			void SetSettings(GameSettings gameSettings);
			void SetTargetFPS(int targetFPS);
			#ifdef StevEngine_SHOW_WINDOW
			void SetVSync(bool vsync);
			void SetFullscreen(bool fullscreen);
			void SetWindowSize(int width, int height);
			#endif
		private:
			#ifdef StevEngine_SHOW_WINDOW
			void Draw();
			void SetSDLWindowSize(int width, int height);
			#endif
			void Update(double deltaTime);
			double currentFPS;
			SDL_Event ev;
			GameSettings gameSettings;
			void SetGameSettingsFromFile();
			#ifdef StevEngine_RENDERER_GL
			SDL_GLContext context;
			#endif
			void (*mainUpdate)(double deltaTime);
	};
}
