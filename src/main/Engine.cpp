#include "Engine.hpp"
//STD
#include <SDL.h>
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
#include "audio/System.hpp"
#include "main/DataManager.hpp"
#include "main/EventSystem.hpp"
#include "main/EngineEvents.hpp"
#include "main/InputSystem.hpp"
#include "main/ResourceManager.hpp"
#include "main/Settings.hpp"
#include "main/multithreading/JobQueue.hpp"
#include "physics/System.hpp"
#include "scenes/GameObject.hpp"
#include "main/Log.hpp"
#include "scenes/Scene.hpp"
#include "scenes/SceneManager.hpp"
#include "visuals/Camera.hpp"
#include "visuals/render/System.hpp"

//Get current process time in ms
uint64_t GetTime() {
	return SDL_GetPerformanceCounter();
}

namespace StevEngine {
	Engine* engine = nullptr;
	Engine::Engine(std::string title, GameSettings gameSettings, bool multithreading)
		: title(title), running(false),gameSettings(gameSettings), events(EventManager()), jobs(JobQueue(multithreading))
	{
		//Initialize logging
		#ifdef StevEngine_PLAYER_DATA
		Log::StartLogging(data.GetLogPath());
		#endif
		//Read settings from file
		#ifdef StevEngine_SETTINGS
		SetGameSettingsFromFile();
		#endif
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
		//Create SDL window
		#ifdef StevEngine_SHOW_WINDOW
		Uint32 SDL_WINDOW_TYPE;
		#ifdef StevEngine_RENDERER_GL
		SDL_WINDOW_TYPE = Render::RenderSystem::WindowType();
		#endif
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gameSettings.WIDTH, gameSettings.HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_TYPE );
		if (!window) {
			throw std::runtime_error("Failed to create window: " + std::string(SDL_GetError()));
		}
		SetFullscreen(gameSettings.fullscreen);
		#endif
		//Done creating engine
		Log::Normal("Initialized Engine", true);
	}

	int Engine::Start() {
		events.Publish(EngineStartEvent());
		//Main loop
		uint64_t lastUpdateTime = GetTime();
		running = true;
		while (running) {
			//Event loop
			while (SDL_PollEvent(&ev) != 0) {
				events.Publish(SDLEvent(ev));
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
								events.Publish(WindowResizeEvent(ev.window.data1, ev.window.data2));
								gameSettings.WIDTH  = ev.window.data1;
								gameSettings.HEIGHT = ev.window.data2;
								#ifdef StevEngine_SETTINGS
								settings.Save("WindowWidth",  gameSettings.WIDTH);
								settings.Save("WindowHeight", gameSettings.HEIGHT);
								#endif
								break;
							case SDL_WINDOWEVENT_MOVED:
								//Log::Debug(std::format("Moving window to {},{}", ev.window.data1, ev.window.data2), true);
								events.Publish(WindowMoveEvent(ev.window.data1, ev.window.data2));
								break;
							case SDL_WINDOWEVENT_DISPLAY_CHANGED:
								if(!gameSettings.fullscreen) break;
								//Log::Debug("Moving window to display " + std::to_string(ev.window.data1), true);
								SDL_Rect bounds;
								SDL_GetDisplayBounds(ev.window.data1, &bounds);
								SDL_SetWindowPosition(window, bounds.x, bounds.y);
								SetWindowSize(bounds.w, bounds.h);
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
				}
			}

			//Calculate delta time
			uint64_t newTime = GetTime();
			uint64_t frameTime = newTime - lastUpdateTime;
			lastUpdateTime = newTime;
			double frameMs = frameTime / (SDL_GetPerformanceFrequency() / 1000.0);

			//Run update
			double frameSeconds = frameMs / 1000.0;
			events.Publish(UpdateEvent(frameSeconds));

			//Draw the frame
			#ifdef StevEngine_SHOW_WINDOW
			events.Publish(EngineDrawEvent());
			#endif

			//Wait for all threads
			jobs.WaitForAllJobs();

			//OpenGL render
			#ifdef StevEngine_RENDERER_GL
			Render::render.DrawFrame();
			#endif

			//Calculate FPS:
			if (frameTime != 0) currentFPS = 1000.0 / frameMs;
			else currentFPS = INFINITY;
			//Log::Debug(std::format("Current FPS: {}; frameTime: {}; Clock: {}", std::round(currentFPS), frameTime, newTime), true);

			//Wait for next frame
			#ifdef StevEngine_SHOW_WINDOW
				bool vsync = gameSettings.vsync;
			#else
				bool vsync = false;
			#endif
			if (gameSettings.targetFPS != -1 && !vsync) {
				double target = (1000.0 / gameSettings.targetFPS);
				if(frameMs < target) SDL_Delay(target - frameMs);
			}
		}

		events.Publish(EngineQuitEvent());

		//Stop logging
		#ifdef StevEngine_PLAYER_DATA
		Log::CloseLogging();
		#endif
		// Destroy the window
		#ifdef StevEngine_SHOW_WINDOW
		SDL_DestroyWindow(window);
		window = NULL;
		#endif
		// Quit SDL
		SDL_Quit();
		return 0;
	}
	#ifdef StevEngine_SETTINGS
	void Engine::SetGameSettingsFromFile() {
		#ifdef StevEngine_SHOW_WINDOW
		if(settings.HasValue("VSync")) gameSettings.vsync = settings.Read<bool>("VSync");
		if(settings.HasValue("Fullscreen")) gameSettings.fullscreen = settings.Read<bool>("Fullscreen");
		if(settings.HasValue("WindowWidth")) gameSettings.WIDTH = settings.Read<int>("WindowWidth");
		if(settings.HasValue("WindowHeight")) gameSettings.HEIGHT = settings.Read<int>("WindowHeight");
		#endif
		if(settings.HasValue("TargetFPS")) gameSettings.targetFPS = settings.Read<int>("TargetFPS");
		//Audio
		#ifdef StevEngine_AUDIO
		if(settings.HasValue("audio.device")) Audio::audio.SetAudioDevice(settings.Read<std::string>("audio.device").c_str());
		if(settings.HasValue("audio.soundVolume")) Audio::audio.SetSoundsVolume(settings.Read<double>("audio.soundVolume"));
		if(settings.HasValue("audio.musicVolume")) Audio::audio.SetMusicVolume(settings.Read<double>("audio.musicVolume"));
		#endif
	}
	#endif
	void Engine::SetSettings(GameSettings newSettings) {
		#ifdef StevEngine_SHOW_WINDOW
		SetVSync(newSettings.vsync);
		SetFullscreen(newSettings.fullscreen);
		SetWindowSize(newSettings.WIDTH, newSettings.HEIGHT);
		#endif
		SetTargetFPS(newSettings.targetFPS);
		#ifdef StevEngine_SETTINGS
		settings.SaveToFile();
		#endif
	}
	void Engine::SetTargetFPS(int targetFPS) {
		gameSettings.targetFPS = targetFPS;
		#ifdef StevEngine_SETTINGS
		settings.Save("TargetFPS", targetFPS);
		settings.SaveToFile();
		#endif
	}
	#ifdef StevEngine_SHOW_WINDOW
	void Engine::SetVSync(bool vsync) {
		gameSettings.vsync = vsync;
		events.Publish(WindowVSyncEvent(vsync));
		#ifdef StevEngine_SETTINGS
		settings.Save("VSync", vsync);
		settings.SaveToFile();
		#endif
	}
	void Engine::SetFullscreen(bool fullscreen) {
		gameSettings.fullscreen = fullscreen;
		if(!fullscreen) SDL_SetWindowResizable(window, SDL_TRUE);
		if(fullscreen) {
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
			SDL_DisplayMode DM;
			SDL_GetDesktopDisplayMode(SDL_GetWindowDisplayIndex(window), &DM);
			SDL_SetWindowSize(window, DM.w, DM.h);
			events.Publish(WindowResizeEvent(DM.w, DM.h));
		} else {
			SDL_SetWindowFullscreen(window, 0);
			SDL_SetWindowSize(window, gameSettings.WIDTH, gameSettings.HEIGHT);
			events.Publish(WindowResizeEvent(gameSettings.WIDTH, gameSettings.HEIGHT));
		}
		events.Publish(WindowFullscreenEvent(fullscreen));
		#ifdef StevEngine_SETTINGS
		settings.Save("Fullscreen", fullscreen);
		settings.SaveToFile();
		#endif
	}
	void Engine::SetWindowSize(int width, int height) {
		gameSettings.WIDTH = width;
		gameSettings.HEIGHT = height;
		SDL_SetWindowSize(window, width, height);
		events.Publish(WindowResizeEvent(width, height));
		#ifdef StevEngine_SETTINGS
		settings.Save("WindowWidth", width);
		settings.Save("WindowHeight", height);
		settings.SaveToFile();
		#endif
	}
	#endif

	double Engine::getFPS() {
		return currentFPS;
	}

	//Create engine and subsystems function
	void CreateEngine(std::string title, GameSettings gameSettings, bool multithreading) {
		#ifdef StevEngine_PLAYER_DATA
			data.Init(title);
		#endif
		#ifdef StevEngine_SETTINGS
		settings.Init(title);
		#endif
		engine = new Engine(title, gameSettings, multithreading);
		#ifdef StevEngine_PHYSICS
			Physics::physics.Init(JPH::PhysicsSettings());
		#endif
		sceneManager.Init();
		#ifdef StevEngine_RENDERER_GL
			Render::render.Init(engine->window);
		#endif
		#ifdef StevEngine_AUDIO
			Audio::audio.Init();
		#endif
		#ifdef StevEngine_INPUTS
		inputManager.Init();
		#endif
	}
}
