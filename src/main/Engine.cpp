#include "Engine.hpp"
//STD
#include <SDL_stdinc.h>
#include <SDL_timer.h>
#include <SDL_video.h>
#include <iostream>
#include <map>
#include <chrono>
#include <algorithm>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Engine
#include "main/InputSystem.hpp"
#include "main/Settings.hpp"
#include "scenes/GameObject.hpp"
#include "main/Log.hpp"
#include "visuals/Camera.hpp"

//Get current process time in ms
uint64_t GetTime() {
	return SDL_GetPerformanceCounter();
}

namespace StevEngine {
	Engine* engine = nullptr;
	bool running = true;
	Engine::Engine(const char * title, GameSettings gameSettings, void (*mainUpdate)(double deltaTime)) :
	title(title),
	gameSettings(gameSettings),
	mainUpdate(mainUpdate),
	resources(),
	#ifdef StevEngine_RENDERER_GL
	render(),
	#endif
	#ifdef StevEngine_PHYSICS
	physics(),
	#endif
	#ifdef StevEngine_PLAYER_DATA
	data(title),
	settings(title),
	#endif
	#ifdef StevEngine_AUDIO
	audio(),
	#endif
	scenes()
	{
		//Initialize logging
		#ifdef StevEngine_PLAYER_DATA
		Log::StartLogging(data.GetLogPath());
		#endif
		//Read settings from file
		SetGameSettingsFromFile();
		//Initialize SDL
		if (SDL_Init(
			SDL_INIT_EVENTS | SDL_INIT_TIMER
			#ifdef StevEngine_AUDIO
			| SDL_INIT_AUDIO
			#endif
			#ifdef StevEngine_SHOW_WINDOW
			| SDL_INIT_VIDEO
			#endif
			) < 0) {
			throw std::runtime_error("Failed initializing SDL: " + std::string(SDL_GetError()));
		}
		#ifdef StevEngine_SHOW_WINDOW
		Uint32 SDL_WINDOW_TYPE;
		#endif
		//Create SDL window
		#ifdef StevEngine_SHOW_WINDOW
		#ifdef StevEngine_RENDERER_GL
		SDL_WINDOW_TYPE = render.WindowType();
		#endif
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gameSettings.WIDTH, gameSettings.HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_TYPE );
		if (!window) {
			throw std::runtime_error("Failed to create window: " + std::string(SDL_GetError()));
		}
		#endif
		//Initialize renderer
		#ifdef StevEngine_RENDERER_GL
		context = render.Init(window);
		render.SetWindowSize(gameSettings.WIDTH, gameSettings.HEIGHT);
		SetFullscreen(gameSettings.fullscreen);
		render.SetVSync(gameSettings.vsync);
		#endif
		//Done creating engine
		Log::Normal("Initialized Engine", true);
	}

	int Engine::Start() {
		//Activate first scene if none has been activated
		scenes.ActivateDefault();
		//Main loop
		uint64_t lastUpdateTime = GetTime();
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
							case SDL_WINDOWEVENT_RESIZED:
								if(gameSettings.fullscreen) break;
								//Log::Debug(std::format("Resizing window to {},{}", ev.window.data1, ev.window.data2), true);
								#ifdef StevEngine_RENDERER_GL
								render.SetWindowSize(ev.window.data1, ev.window.data2);
								#endif
								gameSettings.WIDTH = ev.window.data1;
								gameSettings.HEIGHT = ev.window.data2;
								settings.settings["WindowWidth"] = gameSettings.WIDTH;
								settings.settings["WindowHeight"] = gameSettings.HEIGHT;
								break;
							case SDL_WINDOWEVENT_MOVED:
								//Log::Debug(std::format("Moving window to {},{}", ev.window.data1, ev.window.data2), true);
								break;
							case SDL_WINDOWEVENT_DISPLAY_CHANGED:
								if(!gameSettings.fullscreen) break;
								//Log::Debug("Moving window to display " + std::to_string(ev.window.data1), true);
								SDL_Rect bounds;
								SDL_GetDisplayBounds(ev.window.data1, &bounds);
								SDL_SetWindowPosition(window, bounds.x, bounds.y);
								SetSDLWindowSize(bounds.w, bounds.h);
								break;
							#ifdef StevEngine_INPUTS
							case SDL_WINDOWEVENT_ENTER:
								//Log::Debug(std::format("Mouse entered the window! Motion: {},{}", ev.motion.x, ev.motion.y), true);
								break;
							case SDL_WINDOWEVENT_LEAVE:
								//Log::Debug(std::format("Mouse left the window! Motion: {},{}", ev.motion.x, ev.motion.y), true);
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
			uint64_t newTime = GetTime();
			uint64_t frameTime = newTime - lastUpdateTime;
			lastUpdateTime = newTime;
			double frameMs = frameTime / (SDL_GetPerformanceFrequency() / 1000.0);

			//Run update
			double frameSeconds = frameMs / 1000.0;
			Update(frameSeconds);
			if(mainUpdate) mainUpdate(frameSeconds);

			//Draw the frame
			#ifdef StevEngine_SHOW_WINDOW
			Draw();
			#endif

			//Calculate FPS:
			if (frameTime != 0) currentFPS = 1000.0 / frameMs;
			else currentFPS = INFINITY;
			//Log::Debug(std::format("Current FPS: {}; frameTime: {}; Clock: {}", std::round(currentFPS), frameTime, newTime), true);

			//Wait for next frame
			if (gameSettings.targetFPS != -1 && !gameSettings.vsync) {
				double target = (1000.0 / gameSettings.targetFPS);
				if(frameMs < target) SDL_Delay(target - frameMs);
				/*int sleepDuration = std::clamp((1000.0 / (double)gameSettings.targetFPS) - (double)frameTime, 0.0, (double)INFINITY);
				if (sleepDuration < 0) sleepDuration = 0;
				SDL_Delay(sleepDuration);*/
			}
		}

		//Destroy all scenes
		scenes.scenes.clear();

		//Stop logging
		#ifdef StevEngine_PLAYER_DATA
		Log::CloseLogging();
		#endif

		// Destroy the window
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
		return 0;
	}

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
		#endif
	}
	#endif

	void Engine::SetGameSettingsFromFile() {
		if(settings.HasValue("VSync")) gameSettings.vsync = settings.Read<bool>("VSync");
		if(settings.HasValue("Fullscreen")) gameSettings.fullscreen = settings.Read<bool>("Fullscreen");
		if(settings.HasValue("WindowWidth")) gameSettings.WIDTH = settings.Read<int>("WindowWidth");
		if(settings.HasValue("WindowHeight")) gameSettings.HEIGHT = settings.Read<int>("WindowHeight");
		if(settings.HasValue("TargetFPS")) gameSettings.targetFPS = settings.Read<int>("TargetFPS");
		//Audio
		if(settings.HasValue("audio.device")) audio.SetAudioDevice(settings.Read<std::string>("audio.device").c_str());
		if(settings.HasValue("audio.soundVolume")) audio.SetSoundsVolume(settings.Read<double>("audio.soundVolume"));
		if(settings.HasValue("audio.musicVolume")) audio.SetMusicVolume(settings.Read<double>("audio.musicVolume"));
	}
	void Engine::SetSettings(GameSettings newSettings) {
		#ifdef StevEngine_SHOW_WINDOW
		SetVSync(newSettings.vsync);
		SetFullscreen(newSettings.fullscreen);
		SetWindowSize(newSettings.WIDTH, newSettings.HEIGHT);
		#endif
		SetTargetFPS(newSettings.targetFPS);
	}
	void Engine::SetTargetFPS(int targetFPS) {
		gameSettings.targetFPS = targetFPS;
		settings.settings["TargetFPS"] = targetFPS;
	}
	#ifdef StevEngine_SHOW_WINDOW
	void Engine::SetVSync(bool vsync) {
		gameSettings.vsync = vsync;
		render.SetVSync(vsync);
		settings.settings["VSync"] = vsync;
	}
	void Engine::SetFullscreen(bool fullscreen) {
		gameSettings.fullscreen = fullscreen;
		if(!fullscreen) SDL_SetWindowResizable(window, SDL_TRUE);
		if(fullscreen) {
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
			SDL_DisplayMode DM;
			SDL_GetDesktopDisplayMode(SDL_GetWindowDisplayIndex(window), &DM);
			SetSDLWindowSize(DM.w, DM.h);
		} else {
			SDL_SetWindowFullscreen(window, 0);
			SetSDLWindowSize(gameSettings.WIDTH, gameSettings.HEIGHT);
		}
		settings.settings["Fullscreen"] = fullscreen;
	}
	void Engine::SetWindowSize(int width, int height) {
		gameSettings.WIDTH = width;
		gameSettings.HEIGHT = height;
		SetSDLWindowSize(width, height);
		settings.settings["WindowWidth"] = width;
		settings.settings["WindowHeight"] = height;
	}
	void Engine::SetSDLWindowSize(int width, int height) {
		SDL_SetWindowSize(window, width, height);
		#ifdef StevEngine_RENDERER_GL
		render.SetWindowSize(width, height);
		#endif
	}
	#endif

	double Engine::getFPS() {
		return currentFPS;
	}
}
