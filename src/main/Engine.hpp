#pragma once
#include <physics/System.hpp>
#include <main/ResourceManager.hpp>
#include <main/DataManager.hpp>
#include <audio/System.hpp>
#include <scenes/SceneManager.hpp>

#include <SDL_opengl.h>

namespace StevEngine {
	class Engine {
		public:
			static Engine* Instance;
			Engine(const char * title = "Game", int targetFPS = 60, bool fullScreen = false, void (*mainUpdate)(double deltaTime) = {}, GLint WIDTH = 1080, GLint HEIGHT = 720);
			int Start();
		private:
			void Draw();
			void Update(double deltaTime);
		public:
			SDL_Window* window;
			Physics::System physics;
			Resources::System resources;
			GameData::System data;
			Audio::System audio;
			SceneManager scenes;
			double getFPS();
			const char * title;
		private:
			int targetFPS;
			double currentFPS;
			bool fullScreen;
			SDL_Event ev;
			SDL_GLContext context;
			GLint WIDTH;
			GLint HEIGHT;
			void (*mainUpdate)(double deltaTime);
	};
}