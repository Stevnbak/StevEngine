#pragma once
#ifdef StevEngine_RENDERER_GL
#include "main/Component.hpp"
#include "utilities/Terrain.hpp"

#include "visuals/renderer/RenderComponent.hpp"

namespace StevEngine::Visuals {
	class TerrainRenderer : public Renderer::RenderComponent {
		friend class StevEngine::GameObject;

		public:
			TerrainRenderer(const Utilities::TerrainData& data, Material material = Material(), bool smooth = true);
			TerrainRenderer(YAML::Node node);
			YAML::Node Export(YAML::Node node) const;
		private:
			const Utilities::TerrainData data;
			const bool smooth;
	};
	inline bool terrainRenderer = CreateComponents::RegisterComponentType<TerrainRenderer>("TerrainRenderer");
}
#endif
