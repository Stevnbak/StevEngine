#include "Engine.h"

#include <string>
#include <iostream>
#include <windows.h>
using namespace std;

namespace StevEngine {
	int targetFPS = 60;
	double lastUpdateTime = 0;
	double t = 0.0;
	double currentTime = clock();

	void Tick(double deltaTime) {
		cout << "Tick with deltaTime: " << deltaTime << "\n";
	}
	void Draw() {
		cout << "Drawing...\n";
	}

	void StartEngine() {
		cout << "Started StevEngine!\n";
		lastUpdateTime = clock();
		while (true) {
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
			Sleep(sleepDuration);
			lastUpdateTime = clock();
		}

	}
}