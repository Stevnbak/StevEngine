#pragma once
#ifdef StevEngine_RENDERER_GL
#include "main/Component.hpp"
#include "utilities/Terrain.hpp"

#include "visuals/renderer/RenderComponent.hpp"

namespace StevEngine::Visuals {
	/**
	 * @brief Component for rendering heightmap terrain
	 *
	 * Handles rendering of terrain generated from heightmap data.
	 * Supports normal mapping and smooth/flat shading.
	 */
	class TerrainRenderer : public Renderer::RenderComponent {
		friend class StevEngine::GameObject;

		public:
			/**
			 * @brief Create terrain renderer
			 * @param data Heightmap terrain data
			 * @param material Surface material
			 * @param smooth Use smooth normal calculation
			 */
			TerrainRenderer(const Utilities::TerrainData& data, Material material = Material(), bool smooth = true);

			/**
			 * @brief Create from serialized data
			 * @param node YAML node with terrain data
			 */
			TerrainRenderer(YAML::Node node);

			/**
			 * @brief Serialize to YAML
			 * @param node Node to serialize into
			 * @return Updated YAML node
			 */
			YAML::Node Export(YAML::Node node) const;

		private:
			const Utilities::TerrainData data;  ///< Heightmap terrain data
			const bool smooth;				  ///< Whether to use smooth normal calculation
	};

	/** @brief Register TerrainRenderer as a component type */
	inline bool terrainRenderer = CreateComponents::RegisterComponentType<TerrainRenderer>("TerrainRenderer");
}
#endif
