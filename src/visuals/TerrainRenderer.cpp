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
			std::vector<Vertex> vertices;
			std::vector<uint32_t> indices;
			int halfSize = data.size / 2;
			for(int x = 0; x < data.size - 1; x++) {
				for(int y = 0; y < data.size - 1; y++) {
					//Position
					Vector3 a = Vector3(data.step * (x + 0 - halfSize), data.points[(y + 0) * data.size + (x + 0)], data.step * (y + 0 - halfSize));
					Vector3 b = Vector3(data.step * (x + 1 - halfSize), data.points[(y + 0) * data.size + (x + 1)], data.step * (y + 0 - halfSize));
					Vector3 c = Vector3(data.step * (x + 0 - halfSize), data.points[(y + 1) * data.size + (x + 0)], data.step * (y + 1 - halfSize));
					Vector3 d = Vector3(data.step * (x + 1 - halfSize), data.points[(y + 1) * data.size + (x + 1)], data.step * (y + 1 - halfSize));
					//UV
					Vector2 aUV = Vector2((x + 0) / (double)data.size, (y + 0) / (double)data.size);
					Vector2 bUV = Vector2((x + 1) / (double)data.size, (y + 0) / (double)data.size);
					Vector2 cUV = Vector2((x + 0) / (double)data.size, (y + 1) / (double)data.size);
					Vector2 dUV = Vector2((x + 1) / (double)data.size, (y + 1) / (double)data.size);
					//Normal
					Vector3 edge1 = b - a;
					Vector3 edge2 = c - a;
					Vector2 deltaUV1 = bUV - aUV;
					Vector2 deltaUV2 = cUV - aUV;
					Vector3 normal = Vector3::Cross(edge1, edge2).Normalized();
					double f = 1.0 / (deltaUV1.X * deltaUV2.Y - deltaUV2.X * deltaUV1.Y);
					Vector3 tangent = Vector3(
						f * (deltaUV2.Y * edge1.X - deltaUV1.Y * edge2.X),
						f * (deltaUV2.Y * edge1.Y - deltaUV1.Y * edge2.Y),
						f * (deltaUV2.Y * edge1.Z - deltaUV1.Y * edge2.Z)
					);

					size_t offset = vertices.size();
					vertices.emplace_back(a, aUV, normal, tangent);
					vertices.emplace_back(b, bUV, normal, tangent);
					vertices.emplace_back(c, cUV, normal, tangent);
					vertices.emplace_back(d, dUV, normal, tangent);

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
