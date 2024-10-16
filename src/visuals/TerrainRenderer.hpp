#pragma once
#ifdef StevEngine_RENDERER_GL
#include "scenes/Component.hpp"
#include "utilities/Terrain.hpp"

#include "visuals/render/RenderComponent.hpp"
#include "visuals/render/Object.hpp"

namespace StevEngine {
	namespace Visuals {
		class TerrainRenderer : public Render::RenderComponent {
			friend class StevEngine::GameObject;

			public:
				TerrainRenderer(const Utilities::TerrainData& data, const Utilities::Color& color = Utilities::Color(255, 255, 255, 255), bool smooth = true, Visuals::Texture surface = Visuals::Texture::empty);
				TerrainRenderer(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
			private:
				const Utilities::TerrainData data;
				const bool smooth;
		};
		inline bool terrainRenderer = CreateComponents::RegisterComponentType<TerrainRenderer>("TerrainRenderer");
	}
}
#endif
