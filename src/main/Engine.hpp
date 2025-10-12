#pragma once
#include "EventSystem.hpp"
#include <SDL.h>

namespace StevEngine {
	/**
	 * @brief Engine configuration settings
	 */
	struct GameSettings {
		#ifdef StevEngine_SHOW_WINDOW
		bool vsync = false;	  ///< Whether to use vertical sync
		bool fullscreen = false; ///< Whether to run fullscreen
		uint16_t MSAA = 4;	 ///< Multisample anti-aliasing samples
		int WIDTH = 800;		///< Window width
		int HEIGHT = 600;	   ///< Window height
		#endif
		int targetFPS = 60;	 ///< Target frames per second (-1 for unlimited)
	};

	/**
	 * @brief Main engine class managing core systems
	 *
	 * Handles initialization, main loop, and cleanup of engine systems.
	 * Manages window, events, timing, and settings.
	 */
	class Engine {
		public:
			/**
			 * @brief Create engine instance
			 * @param title Window title
			 * @param gameSettings Initial engine settings
			 */
			Engine(std::string title, GameSettings gameSettings);

			/**
			 * @brief Start engine main loop
			 * @return Exit code
			 */
			int Start();

			#ifdef StevEngine_SHOW_WINDOW
			SDL_Window* window;  ///< Main game window
			#endif

			/**
			 * @brief Get engine event manager
			 * @return Pointer to event manager
			 */
			EventManager& GetEvents() { return events; };

			/**
			 * @brief Get current FPS
			 * @return Current frames per second
			 */
			double getFPS() const;

			bool running;  ///< Whether engine is running

			/** @brief Game window title */
			const std::string title;

			/**
			 * @brief Get current engine settings
			 * @return Current settings struct
			 */
			GameSettings GetGameSettings() const { return gameSettings; }

			/**
			 * @brief Update engine settings
			 * @param gameSettings New settings to apply
			 */
			void SetSettings(GameSettings gameSettings);

			/**
			 * @brief Set target FPS
			 * @param targetFPS New target (-1 for unlimited)
			 */
			void SetTargetFPS(int targetFPS);

			#ifdef StevEngine_SHOW_WINDOW
			/**
			 * @brief Set vertical sync
			 * @param vsync Whether to enable vsync
			 */
			void SetVSync(bool vsync);

			/**
			 * @brief Set fullscreen mode
			 * @param fullscreen Whether to enable fullscreen
			 */
			void SetFullscreen(bool fullscreen);

			/**
			 * @brief Set window size
			 * @param width New width
			 * @param height New height
			 */
			void SetWindowSize(int width, int height);
			#endif

			/**
			 * @brief Get total time in seconds since engine started
			 */
			float GetGameTime() const;

		private:
			EventManager events;	 ///< Engine event manager
			double currentFPS;	   ///< Current frames per second
			SDL_Event ev;			///< SDL event handler
			GameSettings gameSettings; ///< Current engine settings

			#ifdef StevEngine_PLAYER_DATA
			/**
			 * @brief Load settings from file
			 */
			void SetGameSettingsFromFile();
			#endif
	};

	/** Global engine instance */
	extern Engine* engine;

	/**
	 * @brief Create and initialize engine
	 * @param title Window title
	 * @param gameSettings Initial settings
	 */
	void CreateEngine(std::string title = "Game", GameSettings gameSettings = GameSettings());
}
