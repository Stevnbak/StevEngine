#include "Engine.hpp"
#include "Camera.hpp"
#include <iostream>
#include <map>
#include <chrono>
#include <algorithm> 
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include "Utilities.hpp"
#include "InputSystem.hpp"
#include "GameObject.hpp"
#include "Log.hpp"
#include "ResourceManager.hpp"

using namespace std;
using namespace StevEngine::Utilities;
using namespace StevEngine::InputSystem;

namespace StevEngine {
	int targetFPS = 100;
	double currentFPS;
	std::vector<long> frameTimes;
	bool running = true;
	long lastUpdateTime = clock();
	double t = 0.0;
	long currentTime = clock();

	SDL_Window* window = NULL;
	SDL_Event ev;

	extern GLint WIDTH = 1080, HEIGHT = 720;
	
	void Tick(double deltaTime) {
		///Log::Normal(std::format("DeltaTime: {}", deltaTime), true);
		for (GameObject* object : GameObject::GetGameObjects()) {
			object->Update(deltaTime);
		}
		InputSystem::Update(deltaTime);
	}

	Camera* ActiveCamera = nullptr;

	void Draw() {
		// Clear the colorbuffer 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Drawing
		if (ActiveCamera != nullptr) {
			//Start cam matrix
			glPushMatrix();
			//Set view based on camera:
			ActiveCamera->UpdateView();
			//Draw objects
			for (GameObject* object : GameObject::GetGameObjects()) {
				object->Draw();
			}
			//Reset cam matrix
			glPopMatrix();
		}

		//Refresh OpenGL window
		SDL_GL_SetSwapInterval(0);
		SDL_GL_SwapWindow(window);

		//Calculate current FPS
		long newTime = clock();
		frameTimes.push_back(newTime);
		if (frameTimes.size() > 50) {
			frameTimes.erase(frameTimes.begin());
		}
		double frameDuration = newTime - frameTimes[0];
		if (frameDuration != 0) currentFPS = 1000 / (frameDuration / (double)frameTimes.size());
		else currentFPS = INFINITY;
		///Log::Normal(std::format("Current FPS: {}; frameTime: {}; Clock: {}", std::round(currentFPS), (newTime - lastUpdateTime), clock()), true);
		lastUpdateTime = newTime;
	}

	void StartEngine(const char* title, bool fullScreen, void (*mainUpdate)(double deltaTime), void (*mainStart)()) {
		Log::StartLogging(title);
		Log::Normal("Started StevEngine", true);
		//Load resources
		///ResourceManager::RefreshMetadata();
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

		//Create main camera
		ActiveCamera = GameObject::Create("Main Camera", Vector3d(0, 0, 0), Rotation3d(0, 0, 0))->AddComponent<Camera>();
		ActiveCamera->SetOptions(false, 1, 16 / 9);
		//Main start function
		if (mainStart) mainStart();
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
						switch (ev.window.event) {
							case SDL_WINDOWEVENT_SIZE_CHANGED:
								//Resized window
								Log::Normal("Resizing window", true);
								WIDTH = ev.window.data1;
								HEIGHT = ev.window.data2;
								SDL_SetWindowSize(window, WIDTH, HEIGHT);
								glViewport(0, 0, max(WIDTH, HEIGHT), max(WIDTH, HEIGHT));
								Draw();
								break;
							case SDL_WINDOWEVENT_ENTER:
								Log::Normal(std::format("Mouse entered our window! Motion: {},{}", ev.motion.x, ev.motion.y));
								break;
							case SDL_WINDOWEVENT_LEAVE:
								Log::Normal(std::format("Mouse left our window! Motion: {},{}", ev.motion.x, ev.motion.y));
								break;
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
						MouseMotion(ev.motion.x, ev.motion.y, ev.motion.xrel, ev.motion.yrel);
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
					case SDL_WINDOW_MOUSE_CAPTURE:
						Log::Normal(std::format("Mouse capture: {}, {}", ev.motion.x, ev.motion.y));
						break;
				}
			}

			//Calculate delta time
			long newTime = clock();
			long frameTime = newTime - currentTime;
			currentTime = newTime;
			while (frameTime > 0.0)
			{
				double deltaTime = std::min(frameTime, (long)(targetFPS == -1 ? INFINITY : 1000 / targetFPS));
				//Call tick
				Tick(deltaTime / 1000);
				//Main update callback
				if(mainUpdate) mainUpdate(deltaTime);
				frameTime -= deltaTime;
				t += deltaTime;
			}
			//Draw the frame
			Draw();

			//Wait for next frame
			double sleepDuration = ((double)1000 / (targetFPS)) - (frameTime);
			if (sleepDuration < 0) sleepDuration = 0;
			if (targetFPS != -1) SDL_Delay(sleepDuration);
		}

		//Destroy all game objects
		for (GameObject* object : GameObject::GetGameObjects()) {
			object->Destroy();
		}
		Log::CloseLogging();

		// Destroy the window. This will also destroy the surface
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
		window = NULL;

		// Quit SDL
		SDL_Quit();
	}
}

