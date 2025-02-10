#pragma once
#ifdef StevEngine_RENDERER_GL
#include "main/Component.hpp"
#include "utilities/Terrain.hpp"

#include "visuals/renderer/RenderComponent.hpp"

#define TERRAIN_RENDERER_TYPE "TerrainRenderer"

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
			 * @brief Create terrain renderer from serialized data
			 * @param stream Stream containing serialized component data
			 */
			TerrainRenderer(Stream& stream);

			/**
			 * @brief Get component type
			 * @return Type identifier string
			 */
			std::string GetType() const { return TERRAIN_RENDERER_TYPE; }

			/**
			 * @brief Serialize component to a stream
			 * @param type Type of stream to export to
			 * @return Serialized stream
			 */
			Stream Export(StreamType type) const;

		private:
			const Utilities::TerrainData data;  ///< Heightmap terrain data
			const bool smooth;				  ///< Whether to use smooth normal calculation
	};

	/** @brief Register TerrainRenderer as a component type */
	inline bool terrainRenderer = CreateComponents::RegisterComponentType<TerrainRenderer>(TERRAIN_RENDERER_TYPE);
}
#endif
