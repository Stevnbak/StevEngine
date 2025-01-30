#include "Random.hpp"
#include <ctime>

namespace StevEngine::Utilities {
	bool setseed = true;
	int random() {
		if(setseed) {
			srand((unsigned int)time(NULL));
			setseed = false;
		}
		return rand();
	}

	double GetRandomDouble(double min, double max) {
		int r = random();
		double diff = max - min;
		double d = (double)r / RAND_MAX;
		return d * diff + min;
	}
	double GetRandomDouble(double max) { return GetRandomDouble(0, max); }

	int GetRandomInt(int min, int max) {
		int r = random();
		int diff = max - min;
		double d = (double)r / RAND_MAX;
		return (d * diff) + min;
	}
	int GetRandomInt(int max) { return GetRandomInt(0, max); }
}
