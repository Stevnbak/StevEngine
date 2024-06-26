#include "Engine.hpp"
//STD
#include <iostream>
#include <map>
#include <chrono>
#include <algorithm>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Engine
#include <main/InputSystem.hpp>
#include <scenes/GameObject.hpp>
#include <main/Log.hpp>
#include <visuals/Camera.hpp>

using namespace StevEngine::InputSystem;

//Get current process time in ms
uint GetTime() { return SDL_GetTicks(); }

namespace StevEngine {
	Engine* Engine::Instance = nullptr;
	void Engine::Update(double deltaTime) {
		//Run Jolt physics step
		physics.Update(deltaTime);
		//Input?
		InputSystem::Update(deltaTime);
		//Update GameObjects
		Scene* scene = scenes.GetScene(scenes.active);
		for (Utilities::ID id : scene->GetAllObjects()) {
			scene->GetObject(id)->Update(deltaTime);
		}
	}

	void Engine::Draw() {
		// Clear the colorbuffer 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Drawing
		render.StartFrame();
		if (scenes.GetActiveScene()->activeCamera != nullptr) {
			//Draw objects
			Scene* scene = scenes.GetActiveScene();
			for (Utilities::ID id : scene->GetAllObjects()) {
				if(!scene->GetObject(id)->parent.IsNull()) continue;
				scene->GetObject(id)->Draw(glm::mat4x4(1.0));
			}
		}
		// Render
		render.EndFrame();
		// Refresh OpenGL window
		SDL_GL_SetSwapInterval(0);
		SDL_GL_SwapWindow(window);
	}

	bool running = true;

	Engine::Engine(const char * title, int targetFPS, bool fullScreen, void (*mainUpdate)(double deltaTime), GLint WIDTH, GLint HEIGHT) : 
	title(title),
	targetFPS(targetFPS),
	fullScreen(fullScreen),
	WIDTH(WIDTH),
	HEIGHT(HEIGHT),
	mainUpdate(mainUpdate),
	render(Render::System()),
	physics(Physics::System()),
	resources(Resources::System()),
	data(GameData::System(title)),
	audio(Audio::System()),
	scenes(SceneManager())
	{
		//Create instance
		if(Instance != nullptr) {
			throw("Engine has already been initialized.");
		}
		Instance = this;
		//Initialize logging
		Log::StartLogging(data.directoryPath);
		//Initialize SDL
		if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0) {
			throw("Failed initializing SDL: " + std::string(SDL_GetError()));
		}
		//OpenGL properties
		int context_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
		context_flags |= SDL_GL_CONTEXT_DEBUG_FLAG; //Debug flags
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, context_flags);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3 );
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		//Create SDL window
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | ( fullScreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE ));
		if (!window) {
			throw("Failed to create window: " + std::string(SDL_GetError()));
		}
		context = SDL_GL_CreateContext(window);
		if (!context) {
			throw("Failed to create OpenGL context: " + std::string(SDL_GetError()));
		}
		SDL_GL_MakeCurrent(window, context);
		//Initialize GLAD:
        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            throw("Failed to initialize GLAD");
        }
		Log::Debug(std::format("OpenGL Version: {}.{}", GLVersion.major, GLVersion.minor));
		Log::Debug(std::format("OpenGL Shading Language Version: {}", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION)));
		Log::Debug(std::format("OpenGL Vendor: {}", (char *)glGetString(GL_VENDOR)));
		Log::Debug(std::format("OpenGL Renderer: {}", (char *)glGetString(GL_RENDERER)));
		render.Init();
		//Define the OpenGL viewport dimensions
		GLint size = std::max(WIDTH, HEIGHT);
		glViewport(0, 0, size, size);
		glClearColor(1, 0.9, 1, 1);
		glEnable(GL_DEPTH_TEST);
		//Done creating engine
		Log::Normal("Initialized Engine", true);
	}

	int Engine::Start() {
		//Activate first scene if none has been activated
		scenes.ActivateDefault();
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
								glViewport(0, 0, std::max(WIDTH, HEIGHT), std::max(WIDTH, HEIGHT));
								Draw();
								break;
							case SDL_WINDOWEVENT_ENTER:
								Log::Debug(std::format("Mouse entered our window! Motion: {},{}", ev.motion.x, ev.motion.y), true);
								break;
							case SDL_WINDOWEVENT_LEAVE:
								Log::Debug(std::format("Mouse left our window! Motion: {},{}", ev.motion.x, ev.motion.y), true);
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
						Log::Debug(std::format("Mouse capture: {}, {}", ev.motion.x, ev.motion.y), true);
						break;
				}
			}

			//Calculate delta time
			uint newTime = GetTime();
			uint frameTime = newTime - lastUpdateTime;
			lastUpdateTime = newTime;

			//Run update
			double deltaTime = (double)frameTime / (double)1000;
			Update(deltaTime);
			if(mainUpdate) mainUpdate(deltaTime);

			//Draw the frame
			Draw();

			//Calculate FPS:
			if (frameTime != 0) currentFPS = 1000 / frameTime;
			else currentFPS = INFINITY;
			///Log::Debug(std::format("Current FPS: {}; frameTime: {}; Clock: {}", std::round(currentFPS), frameTime, newTime), true);

			//Wait for next frame
			if (targetFPS != -1) {
				int sleepDuration = std::clamp((1000.0 / (double)targetFPS) - (double)frameTime, 0.0, (double)INFINITY);
				if (sleepDuration < 0) sleepDuration = 0;
				SDL_Delay(sleepDuration);
			}
		}

		//Destroy all scenes
		scenes.scenes.clear();
		
		//Stop logging
		Log::CloseLogging();

		// Destroy the window. This will also destroy the surface
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
		window = NULL;

		audio.CleanUp();

		// Quit SDL
		SDL_Quit();

		//Return int
		return 0;
	}
	
	double Engine::getFPS() {
		return currentFPS;
	}
}
