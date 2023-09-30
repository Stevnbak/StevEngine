#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <SDL.h> 
#include <GL/glew.h> 
#include <SDL_opengl.h>  
#include "Debug.h"

using namespace std;

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
		cout << "Tick with deltaTime: " << deltaTime << endl;
	}

	void Draw() {
		cout << "Drawing frame" << endl;
		// Clear the colorbuffer 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set view:
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if (true)
		{
			//Set the perspective with the appropriate aspect ratio
			glFrustum(-1.0, 1.0, -1.0, 1.0, 5, 100);
		}
		else
		{
			//Set up an orthographic projection with the same near clip plane
			glOrtho(-1.0, 1.0, -1.0, 1.0, 5, 100);
		}
		glMatrixMode(GL_MODELVIEW);

		//Draw test
		glColor3f(1, 0, 0);
		DrawCube();
		DrawBox(0, -1.75, -10, (double)100 / WIDTH, (double)100 / HEIGHT);

		//Draw OpenGL
		SDL_GL_SwapWindow(window);
	}

	void StartEngine(const char * title, bool fullScreen) {
		cout << "Started StevEngine!" << endl;
		//Initialize SDL window
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			cout << "Error initializing SDL: " << SDL_GetError() << endl;
			return;
		}
		//Create SDL window
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | ( fullScreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE ));
		if (!window) {
			cout << "Error creating window: " << SDL_GetError() << endl;
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
			cout << "Failed to initialize GLEW"  << endl;
			return;
		}

		// Define the OpenGL viewport dimensions 
		glViewport(0, 0, WIDTH, HEIGHT);
		glClearColor(1, 0.9, 1, 1);
		glEnable(GL_DEPTH_TEST);

		//Draw first frame
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
					case SDL_WINDOWEVENT:

						if (ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
							//Resized window
							cout << "MESSAGE:Resizing window..." << endl;
							WIDTH = ev.window.data1;
							HEIGHT = ev.window.data2;

							SDL_SetWindowSize(window, ev.window.data1, ev.window.data2);
							glViewport(0, 0, ev.window.data1, ev.window.data2);
							Draw();
						}
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
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
		window = NULL;

		// Quit SDL
		SDL_Quit();
	}
}