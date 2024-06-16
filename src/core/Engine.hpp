#pragma once
#include <visuals/Camera.hpp>
#include <physics/System.hpp>
#include <core/ResourceManager.hpp>
#include <audio/System.hpp>

#include <GL/glew.h>

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
			Camera* activeCamera;
			Physics::System* physics;
			SDL_Window* window;
			Resources::System* resources;
			Audio::System* audio;
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