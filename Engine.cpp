#include <iostream>
#include <windows.h>
#include "SDL.h"

using namespace std;

namespace StevEngine {
	int targetFPS = 60;
	bool running = true;
	double lastUpdateTime = 0;
	double t = 0.0;
	double currentTime = clock();

	SDL_Surface* winSurface = NULL;
	SDL_Window* window = NULL;
	SDL_Event ev;

	void Tick(double deltaTime) {
		cout << "Tick with deltaTime: " << deltaTime << "\n";
	}
	void Draw() {
		cout << "Drawing...\n";
		//Clear window
		SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 255, 255, 255));

		//Refresh window surface
		SDL_UpdateWindowSurface(window);
	}

	void StartEngine() {
		cout << "Started StevEngine!\n";
		//Initialize SDL window
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			cout << "Error initializing SDL: " << SDL_GetError() << endl;
			return;
		}
		//Create SDL window
		window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);
		// Make sure creating the window succeeded
		if (!window) {
			cout << "Error creating window: " << SDL_GetError() << endl;
			return;
		}

		// Get the surface from the window
		winSurface = SDL_GetWindowSurface(window);

		// Make sure getting the surface succeeded
		if (!winSurface) {
			cout << "Error getting surface: " << SDL_GetError() << endl;
			return;
		}

		//Draw window
		Draw();

		//Main loop
		lastUpdateTime = clock();
		while (running) {
			//Event loop
			while (SDL_PollEvent(&ev) != 0) {
				// check event type
				switch (ev.type) {
					case SDL_QUIT:
						// shut down
						running = false;
						break;
				}
			}


			//Calculate delta time
			double newTime = clock();
			double frameTime = newTime - currentTime;
			currentTime = newTime;
			while (frameTime > 0.0)
			{
				float deltaTime = min(frameTime, 1000 / targetFPS);
				//Call tick
				Tick(deltaTime / 1000);
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
		SDL_DestroyWindow(window);
		window = NULL;
		winSurface = NULL;

		// Quit SDL
		SDL_Quit();

	}
}