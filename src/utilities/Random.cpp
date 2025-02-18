#include "Random.hpp"
#include <ctime>

namespace StevEngine::Utilities {

	const bool SetRandomSeed() {
		srand((unsigned int)time(NULL));
		return true;
	}

	bool setseed = SetRandomSeed();

	double GetRandomDouble(double min, double max) {
		int r = rand();
		double diff = max - min;
		double d = (double)r / RAND_MAX;
		return d * diff + min;
	}
	double GetRandomDouble(double max) { return GetRandomDouble(0, max); }

	int GetRandomInt(int min, int max) {
		int r = rand();
		int diff = max - min;
		double d = (double)r / RAND_MAX;
		return (d * diff) + min;
	}
	int GetRandomInt(int max) { return GetRandomInt(0, max); }
}
