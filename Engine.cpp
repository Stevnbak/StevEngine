#include <iostream>
#include <windows.h>
#include <SDL.h> 
#include <GL/glew.h> 
#include <SDL_opengl.h>  

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

	const GLint WIDTH = 1080, HEIGHT = 720;

	void Tick(double deltaTime) {
		cout << "Tick with deltaTime: " << deltaTime << endl;
	}
	void Draw() {
		cout << "Drawing..." << endl;
		// Clear the colorbuffer 
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw OpenGL 
		SDL_GL_SwapWindow(window);
	}

	void StartEngine() {
		cout << "Started StevEngine!" << endl;
		//Initialize SDL window
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			cout << "Error initializing SDL: " << SDL_GetError() << endl;
			return;
		}
		//SDL & OpenGL properties
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3); SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		glewExperimental = GL_TRUE;
		//Create SDL window
		window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		// Make sure creating the window succeeded
		if (!window) {
			cout << "Error creating window: " << SDL_GetError() << endl;
			return;
		}
		//OpenGL SDL Context
		SDL_GLContext context = SDL_GL_CreateContext(window);
		// Initialize GLEW to setup the OpenGL Function pointers 
		if (glewInit() != GLEW_OK)
		{
			cout << "Failed to initialize GLEW"  << endl;
			return;
		}
		// Define the OpenGL viewport dimensions 
		glViewport(SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT);

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
					case SDL_WINDOWEVENT:

						if (ev.window.event == SDL_WINDOWEVENT_RESIZED) {
							//Resized window
							cout << "MESSAGE:Resizing window..." << endl;
							SDL_SetWindowSize(window, ev.window.data1, ev.window.data2);
							glViewport(0, 0, ev.window.data1, ev.window.data2);
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
		winSurface = NULL;

		// Quit SDL
		SDL_Quit();
	}
}