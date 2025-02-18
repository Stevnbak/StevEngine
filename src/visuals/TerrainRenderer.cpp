#ifdef StevEngine_RENDERER_GL
#include "TerrainRenderer.hpp"
#include "visuals/renderer/Object.hpp"
#include "visuals/Material.hpp"
#include "utilities/Vector2.hpp"
#include "utilities/Vector3.hpp"
#include "utilities/Terrain.hpp"

#include <vector>

using namespace StevEngine::Utilities;

namespace StevEngine::Visuals {
	Renderer::Object CreateRenderObject(const TerrainData& data, Material material, bool smooth) {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		double halfSize = (data.size - 1) / 2.0;
		for(int x = 0; x < data.size - 1; x++) {
			for(int y = 0; y < data.size - 1; y++) {
				//Position
				Vector3 a = Vector3(data.step * (x + 0 - halfSize), data.points[(y + 0) * data.size + (x + 0)], data.step * (y + 0 - halfSize));
				Vector3 b = Vector3(data.step * (x + 0 - halfSize), data.points[(y + 1) * data.size + (x + 0)], data.step * (y + 1 - halfSize));
				Vector3 c = Vector3(data.step * (x + 1 - halfSize), data.points[(y + 1) * data.size + (x + 1)], data.step * (y + 1 - halfSize));
				Vector3 d = Vector3(data.step * (x + 1 - halfSize), data.points[(y + 0) * data.size + (x + 1)], data.step * (y + 0 - halfSize));
				//UV
				Vector2 aUV = Vector2((x + 0) / (double)data.size, (y + 0) / (double)data.size);
				Vector2 bUV = Vector2((x + 0) / (double)data.size, (y + 1) / (double)data.size);
				Vector2 cUV = Vector2((x + 1) / (double)data.size, (y + 1) / (double)data.size);
				Vector2 dUV = Vector2((x + 1) / (double)data.size, (y + 0) / (double)data.size);
				//Tangent
				Vector3 edge1 = c - a;
				Vector3 edge2 = b - a;
				Vector2 deltaUV1 = cUV - aUV;
				Vector2 deltaUV2 = bUV - aUV;
				double f = 1.0 / (deltaUV1.X * deltaUV2.Y - deltaUV2.X * deltaUV1.Y);
				Vector3 tangent = Vector3(
					f * (deltaUV2.Y * edge1.X - deltaUV1.Y * edge2.X),
					f * (deltaUV2.Y * edge1.Y - deltaUV1.Y * edge2.Y),
					f * (deltaUV2.Y * edge1.Z - deltaUV1.Y * edge2.Z)
				);

				//Create vertices & normals
				size_t offset = vertices.size();
				if(smooth) {
					for(int o = 0; o <= 1; o++) {
						for(int p = 0; p <= 1; p++) {
							double height = data.points[(y + o) * data.size + (x + p)];
							double heightRight	= (x + p != 3 ? data.points[(y + o) * data.size + (x + p + 1)] : (height + (height - data.points[o * data.size + (x + p - 1)])));
							double heightLeft	= (x + p != 0 ? data.points[o * data.size + (x + p - 1)] : (height + (height - heightRight)));
							double heightUp	= (y + o != 3 ? data.points[(y + o + 1) * data.size + (x + p)] : (height + (height - data.points[(y + o - 1) * data.size + (x + p)])));
							double heightDown	= (y + 0 != 0 ? data.points[(y + o - 1) * data.size + (x + p)] : (height + (height - heightUp)));

							Vector3 pointNormal = Vector3(heightLeft - heightRight, 2.0, heightDown - heightUp).Normalized();
							if(p == 0) {
								if(o == 0) {
									vertices.emplace_back(a, aUV, pointNormal, tangent);
								} else {
									vertices.emplace_back(b, bUV, pointNormal, tangent);
								}
							} else {
								if(o == 0) {
									vertices.emplace_back(d, dUV, pointNormal, tangent);
								} else {
									vertices.emplace_back(c, cUV, pointNormal, tangent);
								}
							}
						}
					}

					indices.emplace_back(offset + 0);
					indices.emplace_back(offset + 1);
					indices.emplace_back(offset + 3);

					indices.emplace_back(offset + 0);
					indices.emplace_back(offset + 3);
					indices.emplace_back(offset + 2);
				} else {
					Vector3 faceNormal = Vector3::Cross(edge2, edge1).Normalized();
					vertices.emplace_back(a, aUV, faceNormal, tangent);
					vertices.emplace_back(b, bUV, faceNormal, tangent);
					vertices.emplace_back(c, cUV, faceNormal, tangent);
					vertices.emplace_back(d, dUV, faceNormal, tangent);

					indices.emplace_back(offset + 0);
					indices.emplace_back(offset + 1);
					indices.emplace_back(offset + 2);

					indices.emplace_back(offset + 0);
					indices.emplace_back(offset + 2);
					indices.emplace_back(offset + 3);
				}
			}
		}
		return Renderer::Object(vertices, indices, material);
	}
	TerrainRenderer::TerrainRenderer(const TerrainData& data, Material material, bool smooth)
	  : data(data), smooth(smooth), RenderComponent(CreateRenderObject(data, material, smooth)) {}

	TerrainRenderer::TerrainRenderer(Utilities::Stream& stream)
	  : RenderComponent(Renderer::Object({}, {}), stream), data(TerrainData(stream.Read<TerrainData>())), smooth(stream.Read<bool>())
	{
		object = CreateRenderObject(data, object.material, smooth);
	}

	Utilities::Stream TerrainRenderer::Export(Utilities::StreamType type) const {
		Utilities::Stream stream(type);
		stream << RenderComponent::Export(type) << data << smooth;
		return stream;
	}
}
#endif
