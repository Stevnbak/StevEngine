#include "Engine.hpp"
//STD
#include <iostream>
#include <map>
#include <chrono>
#include <algorithm>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Engine
#include "main/InputSystem.hpp"
#include "scenes/GameObject.hpp"
#include "main/Log.hpp"
#include "visuals/Camera.hpp"

//Get current process time in ms
unsigned int GetTime() { return SDL_GetTicks(); }

namespace StevEngine {
	Engine* Engine::Instance = nullptr;
	void Engine::Update(double deltaTime) {
		#ifdef StevEngine_PHYSICS
		//Run Jolt physics step
		physics.Update(deltaTime);
		#endif
		#ifdef StevEngine_INPUTS
		//Input?
		InputSystem::Update(deltaTime);
		#endif
		//Update GameObjects
		Scene* scene = scenes.GetScene(scenes.active);
		for (Utilities::ID id : scene->GetAllObjects()) {
			scene->GetObject(id)->Update(deltaTime);
		}
	}

	#ifdef StevEngine_SHOW_WINDOW
	void Engine::Draw() {
        // Add objects to render queues
        if (scenes.GetActiveScene()->activeCamera != nullptr) {
			Scene* scene = scenes.GetActiveScene();
			for (Utilities::ID id : scene->GetAllObjects()) {
				if(!scene->GetObject(id)->parent.IsNull()) continue;
				scene->GetObject(id)->Draw(glm::mat4x4(1.0));
			}
        }
		#ifdef StevEngine_RENDERER_GL
		// Render frame
		render.DrawFrame();
		// Refresh OpenGL window
		SDL_GL_SetSwapInterval(0);
		SDL_GL_SwapWindow(window);
		#endif
	}
	#endif

	bool running = true;

	Engine::Engine(
		const char * title
		,int targetFPS
		,void (*mainUpdate)(double deltaTime)
		#ifdef StevEngine_SHOW_WINDOW
		,bool fullScreen
		,int WIDTH
		,int HEIGHT
		#endif
	) :
	title(title),
	targetFPS(targetFPS),
	mainUpdate(mainUpdate),
	#ifdef StevEngine_SHOW_WINDOW
	fullScreen(fullScreen),
	WIDTH(WIDTH),
	HEIGHT(HEIGHT),
	#endif
	resources(Resources::System()),
	#ifdef StevEngine_RENDERER_GL
	render(Render::System()),
	#endif
	#ifdef StevEngine_PHYSICS
	physics(Physics::System()),
	#endif
	#ifdef StevEngine_PLAYER_DATA
	data(GameData::System(title)),
	#endif
	#ifdef StevEngine_AUDIO
	audio(Audio::System()),
	#endif
	scenes(SceneManager())
	{
		//Create instance
		if(Instance != nullptr) {
			throw("Engine has already been initialized.");
		}
		Instance = this;
		//Initialize logging
		#ifdef StevEngine_PLAYER_DATA
		Log::StartLogging(data.directoryPath);
		#endif
		//Initialize SDL
		if (SDL_Init(
			SDL_INIT_EVENTS
			#ifdef StevEngine_AUDIO
			| SDL_INIT_AUDIO
			#endif
			| SDL_INIT_TIMER
			#ifdef StevEngine_SHOW_WINDOW
			| SDL_INIT_VIDEO
			#endif
			) < 0) {
			throw("Failed initializing SDL: " + std::string(SDL_GetError()));
		}
		#ifdef StevEngine_RENDERER_GL
		//OpenGL properties
		int context_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
		#ifdef StevEngine_DEBUGGING
		context_flags |= SDL_GL_CONTEXT_DEBUG_FLAG; //Debug flags
		#endif
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
		#endif
		#ifdef StevEngine_SHOW_WINDOW
		//Create SDL window
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | ( fullScreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE ));
		if (!window) {
			throw("Failed to create window: " + std::string(SDL_GetError()));
		}
		#endif
		#ifdef StevEngine_RENDERER_GL
		context = SDL_GL_CreateContext(window);
		if (!context) {
			throw("Failed to create OpenGL context: " + std::string(SDL_GetError()));
		}
		SDL_GL_MakeCurrent(window, context);
		//Initialize renderer
		render.Init();
		//Define the OpenGL viewport dimensions
		GLint size = std::max(WIDTH, HEIGHT);
		glViewport(0, 0, size, size);
		#endif
		//Done creating engine
		Log::Normal("Initialized Engine", true);
	}

	int Engine::Start() {
		//Activate first scene if none has been activated
		scenes.ActivateDefault();
		//Main loop
		unsigned int lastUpdateTime = GetTime();
		while (running) {
			#ifdef StevEngine_INPUTS
			//Reset mouse delta
			InputSystem::ResetMouseDelta();
			#endif
			//Event loop
			while (SDL_PollEvent(&ev) != 0) {
				// check event type
				switch (ev.type) {
					case SDL_QUIT:
						// shut down
						running = false;
						break;
					#ifdef StevEngine_SHOW_WINDOW
					case SDL_WINDOWEVENT:
						switch (ev.window.event) {
							case SDL_WINDOWEVENT_SIZE_CHANGED:
								//Resized window
								Log::Normal("Resizing window", true);
								WIDTH = ev.window.data1;
								HEIGHT = ev.window.data2;
								SDL_SetWindowSize(window, WIDTH, HEIGHT);
								#ifdef StevEngine_RENDERER_GL
								glViewport(0, 0, std::max(WIDTH, HEIGHT), std::max(WIDTH, HEIGHT));
								#endif
								Draw();
								break;
							#ifdef StevEngine_INPUTS
							case SDL_WINDOWEVENT_ENTER:
								Log::Debug(std::format("Mouse entered our window! Motion: {},{}", ev.motion.x, ev.motion.y), true);
								break;
							case SDL_WINDOWEVENT_LEAVE:
								Log::Debug(std::format("Mouse left our window! Motion: {},{}", ev.motion.x, ev.motion.y), true);
								break;
							#endif
						}
						break;
					#endif
					#ifdef StevEngine_INPUTS
					//Input system
					case SDL_KEYDOWN:
						InputSystem::KeyDown(ev.key.keysym.sym);
						break;
					case SDL_KEYUP:
						InputSystem::KeyUp(ev.key.keysym.sym);
						break;
					case SDL_MOUSEMOTION:
						InputSystem::MouseMotion(ev.motion.x, ev.motion.y, ev.motion.xrel, ev.motion.yrel);
						break;
					case SDL_MOUSEWHEEL:
						InputSystem::MouseWheel(ev.wheel.preciseY);
						break;
					case SDL_MOUSEBUTTONDOWN:
						InputSystem::KeyDown(ev.button.button);
						break;
					case SDL_MOUSEBUTTONUP:
						InputSystem::KeyUp(ev.button.button);
						break;
					case SDL_WINDOW_MOUSE_CAPTURE:
						Log::Debug(std::format("Mouse capture: {}, {}", ev.motion.x, ev.motion.y), true);
						break;
					#endif
				}
			}

			//Calculate delta time
			unsigned int newTime = GetTime();
			unsigned int frameTime = newTime - lastUpdateTime;
			lastUpdateTime = newTime;

			//Run update
			double deltaTime = (double)frameTime / (double)1000;
			Update(deltaTime);
			if(mainUpdate) mainUpdate(deltaTime);

			//Draw the frame
			#ifdef StevEngine_SHOW_WINDOW
			Draw();
			#endif

			//Calculate FPS:
			if (frameTime != 0) currentFPS = 1000.0 / frameTime;
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
		#ifdef StevEngine_PLAYER_DATA
		Log::CloseLogging();
		#endif

		// Destroy the window. This will also destroy the surface
		#ifdef StevEngine_RENDERER_GL
		SDL_GL_DeleteContext(context);
		#endif
		#ifdef StevEngine_SHOW_WINDOW
		SDL_DestroyWindow(window);
		window = NULL;
		#endif
		#ifdef StevEngine_AUDIO
		audio.CleanUp();
		#endif
		// Quit SDL
		SDL_Quit();

		//Return int
		return 0;
	}

	double Engine::getFPS() {
		return currentFPS;
	}
}
