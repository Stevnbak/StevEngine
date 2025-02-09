#include "Terrain.hpp"
#include "utilities/Stream.hpp"
#include <algorithm>
#include <cstdint>

namespace StevEngine::Utilities {
	TerrainData::TerrainData(uint32_t size, double step, double* data) : size(size), step(step), points(new double[size * size]) {
		std::copy(&data[0], &data[(size * size)], &points[0]);
	};
	TerrainData::TerrainData(uint32_t size, double step) : size(size), step(step), points(new double[size * size]) {};
}

namespace StevEngine {
	//Read from text stream
	template <> Utilities::TerrainData Stream::Read<Utilities::TerrainData>() {
		uint32_t size = Read<uint32_t>();
		double step = Read<double>();
		double* data = new double[size * size];
		for(int i = 0; i < size * size; i++) *this >> data[i];
		return Utilities::TerrainData(size, step, data);
	}
	//Write to text stream
	template <> void Stream::Write<Utilities::TerrainData>(const Utilities::TerrainData& data) {
		*this << data.size << data.step;
		for(int i = 0; i < data.size * data.size; i++) *this << data.points[i];
	}
}