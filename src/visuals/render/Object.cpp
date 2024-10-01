#include "main/Log.hpp"
#include "utilities/Vertex.hpp"
#include <memory>
#include <string>
#ifdef StevEngine_RENDERER_GL
#include "Object.hpp"
#include "visuals/Texture.hpp"

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
				result[j++] = vertex.normal.X;
				result[j++] = vertex.normal.Y;
				result[j++] = vertex.normal.Z;
				result[j++] = vertex.uv.X;
				result[j++] = vertex.uv.Y;
			}
			return result;
		}
		Object::Object(const std::vector<Vertex>& vertices, Color color, Visuals::Texture textureData) {
			//Create indices and filter out duplicates
			std::vector<Vertex> uniqueVertices;
			for (Vertex v : vertices) {
				int index = std::find(uniqueVertices.begin(), uniqueVertices.end(), v) - uniqueVertices.begin();
				if(index == uniqueVertices.size()) {
					uniqueVertices.emplace_back(v);
				}
				indices.emplace_back(index);
			}
			//Generate vertex array
			this->vertices = ToFloatList(uniqueVertices);
			//Bind texture
			SetTexture(textureData);
		}
		Object::Object(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, Color color, Visuals::Texture textureData) : vertices(ToFloatList(vertices)), indices(indices) {
			SetTexture(textureData);
		}
		Object::Object(const Object& instance, Utilities::Color color, Visuals::Texture textureData) : indices(instance.indices), vertices(instance.vertices), color(color) {
			SetTexture(textureData);
		}
		void Object::SetTexture(Visuals::Texture textureData) {
			texture = textureData.BindTexture();
			if(texture) {
				textured = true;

			} else {
				textured = false;
			}
		}
		void Object::FreeTexture() {
			glDeleteTextures(1, &texture);
			texture = 0;
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
