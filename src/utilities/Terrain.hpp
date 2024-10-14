#pragma once

#include "yaml-cpp/yaml.h"
#include <cstdint>
namespace StevEngine::Utilities {
	struct TerrainData {
			TerrainData(uint32_t size, double step, double* data);
			TerrainData(uint32_t size, double step);
			YAML::Node Export() const;
			TerrainData(YAML::Node node);
			const uint32_t size;
			const double step;
			double* points;
	};
}