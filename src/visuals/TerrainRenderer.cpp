#ifdef StevEngine_RENDERER_GL
#include "TerrainRenderer.hpp"
#include "visuals/render/Object.hpp"
#include "visuals/Texture.hpp"
#include "utilities/Color.hpp"
#include "utilities/Vector2.hpp"
#include "utilities/Vector3.hpp"

#include <string>
#include <vector>

using namespace StevEngine::Utilities;

namespace StevEngine {
	namespace Visuals {
		Render::Object CreateRenderObject(const TerrainData& data, Color color, Texture surface) {
			std::vector<Utilities::Vertex> vertices;
			std::vector<uint32_t> indices;
			int halfSize = data.size / 2;
			for(int x = 0; x < data.size - 1; x++) {
				for(int y = 0; y < data.size - 1; y++) {
					double u1 = (x) / (double)data.size;
					double u2 = (x + 1) / (double)data.size;
					double v1 = (y) / (double)data.size;
					double v2 = (y + 1) / (double)data.size;

					Utilities::Vector3 a = Utilities::Vector3(data.step * (x + 0 - halfSize), data.points[(y + 0) * data.size + (x + 0)], data.step * (y + 0 - halfSize));
					Utilities::Vector3 b = Utilities::Vector3(data.step * (x + 1 - halfSize), data.points[(y + 0) * data.size + (x + 1)], data.step * (y + 0 - halfSize));
					Utilities::Vector3 c = Utilities::Vector3(data.step * (x + 0 - halfSize), data.points[(y + 1) * data.size + (x + 0)], data.step * (y + 1 - halfSize));
					Utilities::Vector3 d = Utilities::Vector3(data.step * (x + 1 - halfSize), data.points[(y + 1) * data.size + (x + 1)], data.step * (y + 1 - halfSize));

					Utilities::Vector3 normal = Utilities::Vector3::Cross(c - a, b - a).Normalized();

					size_t offset = vertices.size();
					vertices.emplace_back(a, normal, Utilities::Vector2(u1, v1));
					vertices.emplace_back(b, normal, Utilities::Vector2(u2, v1));
					vertices.emplace_back(c, normal, Utilities::Vector2(u1, v2));
					vertices.emplace_back(d, normal, Utilities::Vector2(u2, v2));

					indices.emplace_back(offset + 0);
					indices.emplace_back(offset + 1);
					indices.emplace_back(offset + 2);

					indices.emplace_back(offset + 1);
					indices.emplace_back(offset + 2);
					indices.emplace_back(offset + 3);
				}
			}
			return Render::Object(vertices, indices, color, surface);
		}
		TerrainRenderer::TerrainRenderer(const TerrainData& data, const Color& color, Texture surface)
			: data(data), RenderComponent(CreateRenderObject(data, color, surface), "TerrainRenderer") {}
		TerrainRenderer::TerrainRenderer(YAML::Node node)
			: data(TerrainData(node["terrain"])), RenderComponent(CreateRenderObject(data, node["color"].as<Color>(), Texture::empty), node) {}
		YAML::Node TerrainRenderer::Export(YAML::Node node) const {
			YAML::Node n = RenderComponent::Export(node);
			n["texture"] = data.Export();
			return n;
		}
	}
}
#endif
