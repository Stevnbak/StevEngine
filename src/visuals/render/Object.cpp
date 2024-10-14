#include <cstdint>
#ifdef StevEngine_RENDERER_GL
#include "Object.hpp"
#include "utilities/Vertex.hpp"
#include "visuals/Texture.hpp"
#include "main/Log.hpp"

#include <memory>
#include <string>

#include "glad/gl.h"
#include <algorithm>

using StevEngine::Utilities::Vertex;
using StevEngine::Utilities::Color;

namespace StevEngine {
	namespace Render {
		std::vector<float> ToFloatList(const std::vector<Vertex>& vertices) {
			std::vector<float> result;
			result.resize(vertices.size() * (Utilities::VERTEX_COUNT));
			int j = 0;
			for(int i = 0; i < vertices.size(); i++) {
				const Vertex& vertex = vertices[i];
				result[j++] = vertex.position.X;
				result[j++] = vertex.position.Y;
				result[j++] = vertex.position.Z;
				result[j++] = vertex.uv.X;
				result[j++] = vertex.uv.Y;
				result[j++] = vertex.normal.X;
				result[j++] = vertex.normal.Y;
				result[j++] = vertex.normal.Z;
				result[j++] = vertex.tangent.X;
				result[j++] = vertex.tangent.Y;
				result[j++] = vertex.tangent.Z;
			}
			return result;
		}
		Object::Object(const std::vector<Vertex>& vertices, const Color& color, const Visuals::Texture& textureData, const Visuals::Texture& normalMapData)
	 		: texture(textureData), normalMap(normalMapData) {
			//Create indices and filter out duplicates
			std::vector<Vertex> uniqueVertices;
			std::vector<uint32_t> newIndices;
			size_t i = 0;
			for (Vertex v : vertices) {
				int index = std::find(uniqueVertices.begin(), uniqueVertices.end(), v) - uniqueVertices.begin();
				if(index == uniqueVertices.size()) {
					uniqueVertices.emplace_back(v);
				}
				newIndices.emplace_back(index);
			}
			//Fill vertex and index arrays
			auto floatVertices = ToFloatList(uniqueVertices);
			vertexCount = floatVertices.size();
			this->vertices = new float[vertexCount];
			for(int i = 0; i < vertexCount; i++) {
				this->vertices[i] = floatVertices[i];
			}
			indexCount = newIndices.size();
			this->indices = new uint32_t[indexCount];
			for(int i = 0; i < indexCount; i++) {
				this->indices[i] = newIndices[i];
			}
			//Bind texture
			SetTexture(textureData);
		}
		Object::Object(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const Utilities::Color& color, const Visuals::Texture& textureData, const Visuals::Texture& normalMapData)
			: vertices(new float[vertices.size()]), vertexCount(vertices.size()), indices(new uint32_t[indices.size()]), indexCount(indices.size())
		{
			auto floatVertices = ToFloatList(vertices);
			vertexCount = floatVertices.size();
			this->vertices = new float[vertexCount];
			for(int i = 0; i < vertexCount; i++) {
				this->vertices[i] = floatVertices[i];
			}
			indexCount = indices.size();
			this->indices = new uint32_t[indexCount];
			for(int i = 0; i < indexCount; i++) {
				this->indices[i] = indices[i];
			}
			SetTexture(textureData);
			SetTexture(normalMapData);
		}
		Object::Object(const Object& instance, const Color& color, const Visuals::Texture& textureData, const Visuals::Texture& normalMapData)
			: indices(instance.indices), indexCount(instance.indexCount), vertices(instance.vertices), vertexCount(instance.vertexCount), color(color)
		{
			SetTexture(textureData);
			SetTexture(normalMapData);
		}
		void Object::SetTexture(const Visuals::Texture& textureData) {
			texture = textureData;
			texture.BindTexture();
		}
		void Object::SetNormalMap(const Visuals::Texture& normalData) {
			normalMap = normalData;
			normalMap.BindTexture();
		}
		void Object::FreeTexture() {
			texture.FreeTexture();
		}
		void Object::FreeNormalMap() {
			normalMap.FreeTexture();
		}
		void Object::AddShader(Render::ShaderProgram program) {
			if(shaders.contains(program.GetType())) RemoveShader(program.GetType());
			program.RelinkProgram();
			shaders.insert({ program.GetType(), program });
		}
		void Object::RemoveShader(Render::ShaderType type) {
			shaders.erase(shaders.find(type));
		}
	}
}
#endif
