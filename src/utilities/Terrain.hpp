#pragma once
#include "yaml-cpp/yaml.h"
#include <cstdint>

namespace StevEngine::Utilities {
	/**
	 * @brief Heightmap terrain data storage
	 *
	 * Stores and manages heightmap data for terrain generation.
	 * Includes grid size, spacing, and height values.
	 */
	struct TerrainData {
			/**
			 * @brief Create terrain with existing data
			 * @param size Grid size (NxN)
			 * @param step Grid spacing
			 * @param data Array of height values
			 */
			TerrainData(uint32_t size, double step, double* data);

			/**
			 * @brief Create empty terrain
			 * @param size Grid size (NxN)
			 * @param step Grid spacing
			 */
			TerrainData(uint32_t size, double step);

			/**
			 * @brief Serialize terrain to YAML
			 * @return YAML node containing terrain data
			 */
			YAML::Node Export() const;

			/**
			 * @brief Create terrain from serialized data
			 * @param node YAML node containing terrain data
			 */
			TerrainData(YAML::Node node);

			const uint32_t size;  ///< Grid size (NxN)
			const double step;	///< Grid spacing
			double* points;	   ///< Height values array
	};
}