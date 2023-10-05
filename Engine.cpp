#include "Engine.hpp"
#include "Camera.hpp"
#include <iostream>
#include <map>
#include <algorithm> 
#include <SDL2/include/SDL.h>
#include <GLEW/include/GL/glew.h>
#include <SDL2/include/SDL_opengl.h>
#include "Utilities.hpp"
#include "InputSystem.hpp"
#include "GameObject.hpp"
#include "Log.hpp"

using namespace std;
using namespace StevEngine::Utilities;
using namespace StevEngine::InputSystem;


namespace StevEngine {
	int targetFPS = 60;
	bool running = true;
	double lastUpdateTime = 0;
	double t = 0.0;
	double currentTime = clock();

	SDL_Window* window = NULL;
	SDL_Event ev;

	extern GLint WIDTH = 1080, HEIGHT = 720;
	
	void Tick(double deltaTime) {
		for (GameObject* object : GameObject::GetGameObjects()) {
			object->Update(deltaTime);
		}
	}
	Camera ActiveCamera (Vector3d(4,5,10), Vector3d(-20, 40, 0), false, 1, 16 / 9);

	void Draw() {
		// Clear the colorbuffer 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glPushMatrix();

		//Set view based on camera:
		ActiveCamera.UpdateView();

		//Draw objects
		for (GameObject* object : GameObject::GetGameObjects()) {
			object->Draw();
		}

		glPopMatrix();
		//Draw OpenGL
		SDL_GL_SwapWindow(window);
	}

	void StartEngine(const char* title, bool fullScreen, void (*mainUpdate)(double deltaTime), void (*mainStart)()) {
		Log::Normal("Started StevEngine", true);
		//Initialize SDL window
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			Log::Error("Failed initializing SDL: " + char(SDL_GetError()), true);
			return;
		}
		//Create SDL window
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | ( fullScreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE ));
		if (!window) {
			Log::Error("Failed to create window: " + char(SDL_GetError()), true);
			return;
		}
		//SDL & OpenGL properties
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		glewExperimental = GL_TRUE;
		SDL_GLContext context = SDL_GL_CreateContext(window);
		// Initialize GLEW to setup the OpenGL Function pointers 
		if (glewInit() != GLEW_OK)
		{
			Log::Error("Failed to initialize GLEW ", true);
			return;
		}

		// Define the OpenGL viewport dimensions
		GLint size = max(WIDTH, HEIGHT);
		glViewport(0, 0, size, size);
		glClearColor(1, 0.9, 1, 1);
		glEnable(GL_DEPTH_TEST);

		//Main start function
		mainStart();
		//Draw first frame
		Draw();

		//Main loop
		lastUpdateTime = clock();
		while (running) {
			//Reset mouse delta
			ResetMouseDelta();
			//Event loop
			while (SDL_PollEvent(&ev) != 0) {
				// check event type
				switch (ev.type) {
					case SDL_QUIT:
						// shut down
						running = false;
						break;
					case SDL_WINDOWEVENT:

						if (ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
							//Resized window
							Log::Normal("Resizing window", true);
							WIDTH = ev.window.data1;
							HEIGHT = ev.window.data2;
							SDL_SetWindowSize(window, WIDTH, HEIGHT);
							GLint size = max(WIDTH, HEIGHT);
							glViewport(0, 0, size, size);
							Draw();
						}
						break;
					//Input system
					case SDL_KEYDOWN:
						KeyDown(ev.key.keysym.sym);
						break;
					case SDL_KEYUP:
						KeyUp(ev.key.keysym.sym);
						break;
					case SDL_MOUSEMOTION:
						MouseMotion(ev.motion.x, ev.motion.y);
						break;
					case SDL_MOUSEWHEEL:
						MouseWheel(ev.wheel.preciseY);
						break;
					case SDL_MOUSEBUTTONDOWN:
						KeyDown(ev.button.button);
						break;
					case SDL_MOUSEBUTTONUP:
						KeyUp(ev.button.button);
						break;

				}
			}

			//Calculate delta time
			double newTime = clock();
			double frameTime = newTime - currentTime;
			currentTime = newTime;
			while (frameTime > 0.0)
			{
				float deltaTime = std::fmin(frameTime, 1000 / targetFPS);
				//Call tick
				Tick(deltaTime / 1000);
				//Main update callback
				mainUpdate(deltaTime);
				frameTime -= deltaTime;
				t += deltaTime;
			}
			//Draw the frame
			Draw();

			//Wait for next frame
			double sleepDuration = (1000 / targetFPS) - (clock() - lastUpdateTime);
			if (sleepDuration < 0) sleepDuration = 0;
			SDL_Delay(sleepDuration);
			///Sleep(sleepDuration);
			lastUpdateTime = clock();
		}

		// Destroy the window. This will also destroy the surface
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
		window = NULL;

		// Quit SDL
		SDL_Quit();
	}
}

