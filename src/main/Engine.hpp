#pragma once
#include "EventSystem.hpp"
#include "Log.hpp"
#include <SDL.h>
#include <sys/types.h>

namespace StevEngine {
	struct GameSettings {
		#ifdef StevEngine_SHOW_WINDOW
		bool vsync = false;
		bool fullscreen = false;
		u_int16_t MSAA = 4;
		int WIDTH = 800;
		int HEIGHT = 600;
		#endif
		int targetFPS = 60;
	};
	class Engine {
		public:
			Engine(std::string title, GameSettings gameSettings);
			int Start();
			#ifdef StevEngine_SHOW_WINDOW
			SDL_Window* window;
			#endif
			EventManager* GetEvents() { return &events; };
			double getFPS() const;
			bool running;
			//Engine settings
			const std::string title;
			GameSettings GetGameSettings() const { return gameSettings; }
			void SetSettings(GameSettings gameSettings);
			void SetTargetFPS(int targetFPS);
			#ifdef StevEngine_SHOW_WINDOW
			void SetVSync(bool vsync);
			void SetFullscreen(bool fullscreen);
			void SetWindowSize(int width, int height);
			#endif
		private:
			EventManager events;
			double currentFPS;
			SDL_Event ev;
			GameSettings gameSettings;
			#ifdef StevEngine_PLAYER_DATA
			void SetGameSettingsFromFile();
			#endif
	};

	extern Engine* engine;
	void CreateEngine(std::string title = "Game", GameSettings gameSettings = GameSettings());
}
