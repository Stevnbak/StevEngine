#include "Engine.hpp"
//STD
#include <iostream>
#include <map>
#include <chrono>
#include <algorithm> 
//Libraries
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <Jolt/Jolt.h>
//Engine
#include <Core/Utilities.hpp>
#include <Core/InputSystem.hpp>
#include <Core/GameObject.hpp>
#include <Core/Log.hpp>
#include <Core/ResourceManager.hpp>
#include <Physics/System.hpp>
#include <Visuals/Camera.hpp>

using namespace std;
using namespace StevEngine::Utilities;
using namespace StevEngine::InputSystem;

//Get current process time in ms
uint GetTime() {
	return SDL_GetTicks();
	/**
	return std::round(((double)1000 / (targetFPS)) - (frameTime));
	*/
}

namespace StevEngine {
	int targetFPS = 100;
	double currentFPS = 0;
	bool running = true;
	double t = 0.0;

	SDL_Window* window = NULL;
	SDL_Event ev;
	Physics::System* physics;

	GLint WIDTH = 1080, HEIGHT = 720;
	
	void Tick(double deltaTime) {
		//Run Jolt Physics step
		physics->Update(deltaTime);
		//Input?
		InputSystem::Update(deltaTime);
		//Update GameObjects
		for (GameObject* object : GameObject::GetGameObjects()) {
			object->Update(deltaTime);
		}
	}

	Camera* ActiveCamera = nullptr;

	void Draw() {
		// Clear the colorbuffer 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Drawing
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
		// Refresh OpenGL window
		SDL_GL_SetSwapInterval(0);
		SDL_GL_SwapWindow(window);
	}

	int StartEngine(const char* title, bool fullScreen, void (*mainUpdate)(double deltaTime), void (*mainStart)()) {
		Log::StartLogging(title);
		Log::Normal("Started StevEngine", true);
		//Load resources
		///ResourceManager::RefreshMetadata();
		//Initialize SDL window
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			Log::Error("Failed initializing SDL: " + string(SDL_GetError()), true);
			return 1;
		}
		//Create SDL window
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | ( fullScreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE ));
		if (!window) {
			Log::Error("Failed to create window: " + string(SDL_GetError()), true);
			return 1;
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
			return 1;
		}

		//Create physics system
		physics = new Physics::System();

		// Define the OpenGL viewport dimensions
		GLint size = max(WIDTH, HEIGHT);
		glViewport(0, 0, size, size);
		glClearColor(1, 0.9, 1, 1);
		glEnable(GL_DEPTH_TEST);

		//Create main camera
		ActiveCamera = GameObject::Create("Main Camera", Vector3d(0, 0, 0), Rotation3d(0, 0, 0))->AddComponent(new Camera());
		ActiveCamera->SetOptions(false, 1, 16 / 9);
		//Main start function
		if (mainStart) mainStart();
		//Main loop
		uint lastUpdateTime = GetTime();
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
								Log::Normal(std::format("Mouse entered our window! Motion: {},{}", ev.motion.x, ev.motion.y), true);
								break;
							case SDL_WINDOWEVENT_LEAVE:
								Log::Normal(std::format("Mouse left our window! Motion: {},{}", ev.motion.x, ev.motion.y), true);
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
			uint newTime = GetTime();
			uint frameTime = newTime - lastUpdateTime;
			lastUpdateTime = newTime;

			//Run update
			double deltaTime = (double)frameTime / (double)1000;
			Tick(deltaTime);
			if(mainUpdate) mainUpdate(deltaTime);

			//Draw the frame
			Draw();

			//Calculate FPS:
			if (frameTime != 0) currentFPS = 1000 / frameTime;
			else currentFPS = INFINITY;
			///Log::Normal(std::format("Current FPS: {}; frameTime: {}; Clock: {}", std::round(currentFPS), frameTime, newTime), true);

			//Wait for next frame
			if (targetFPS != -1) {
				int sleepDuration = std::clamp((1000.0 / (double)targetFPS) - (double)frameTime, 0.0, (double)INFINITY);
				if (sleepDuration < 0) sleepDuration = 0;
				SDL_Delay(sleepDuration);
			}
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

		delete physics;

		// Quit SDL
		SDL_Quit();

		//Return int
		return 0;
	}
}
