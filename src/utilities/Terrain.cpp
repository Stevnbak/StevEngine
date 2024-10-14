#include "Terrain.hpp"
#include <algorithm>
#include <cstdint>

namespace StevEngine::Utilities {
	TerrainData::TerrainData(uint32_t size, double step, double* data) : size(size), step(step), points(new double[size * size]) {
		std::copy(&data[0], &data[(size * size)], &points[0]);
	};
	TerrainData::TerrainData(uint32_t size, double step) : size(size), step(step), points(new double[size * size]) {};

	YAML::Node TerrainData::Export() const {
		YAML::Node node;
		node.push_back(size);
		node.push_back(step);
		for(int i = 0; i < size * size; i++) node.push_back(points[i]);
		return node;
	}
	TerrainData::TerrainData(YAML::Node node) : size(node[0].as<uint32_t>()), step(node[1].as<double>()), points(new double[size * size]) {
		for(int i = 0; i < size * size; i++) {
			points[i] = node[i + 2].as<double>();
		}
	}
}