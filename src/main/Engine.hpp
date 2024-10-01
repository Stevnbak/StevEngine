#pragma once
#include "EventSystem.hpp"
#include "Log.hpp"
#include "main/multithreading/JobQueue.hpp"
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
			Engine(std::string title, GameSettings gameSettings, bool multithreading);
			int Start();
			#ifdef StevEngine_SHOW_WINDOW
			SDL_Window* window;
			#endif
			EventManager& GetEvents() { return events; };
			JobQueue& GetJobs() { return jobs; };
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
			JobQueue jobs;
			double currentFPS;
			SDL_Event ev;
			GameSettings gameSettings;
			#ifdef StevEngine_SETTINGS
			void SetGameSettingsFromFile();
			#endif
	};

	extern Engine* engine;
	void CreateEngine(std::string title = "Game", GameSettings gameSettings = GameSettings(), bool multithreading = true);
}
