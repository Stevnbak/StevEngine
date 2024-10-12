#include "Terrain.hpp"
#include <algorithm>

namespace StevEngine::Utilities {
	TerrainData::TerrainData(uint32_t size, double step, double* data) : size(size), step(step), points(new double[size * size]) {
		std::copy(&data[0], &data[(size * size)], &points[0]);
	};
	TerrainData::TerrainData(uint32_t size, double step) : size(size), step(step), points(new double[size * size]) {};
}