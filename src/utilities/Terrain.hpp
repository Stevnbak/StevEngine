#pragma once

#include <cstdint>
namespace StevEngine::Utilities {
	struct TerrainData {
			TerrainData(uint32_t size, double step, double* data);
			TerrainData(uint32_t size, double step);
			const uint32_t size;
			const double step;
			double* points;
	};
}