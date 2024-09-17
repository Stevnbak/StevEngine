#include "Random.hpp"

namespace StevEngine::Utilities {
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
