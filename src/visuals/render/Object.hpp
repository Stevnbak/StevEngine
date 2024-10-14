#pragma once
#ifdef StevEngine_RENDERER_GL
#include <vector>
#include <cstdint>
#include <SDL.h>
#include <glad/gl.h>
#include <glm/mat4x4.hpp>

#include "utilities/Vector3.hpp"
#include "utilities/Quaternion.hpp"
#include "utilities/Vertex.hpp"
#include "utilities/Color.hpp"
#include "visuals/Texture.hpp"
#include "visuals/shaders/ShaderProgram.hpp"

namespace StevEngine {
	namespace Render {
		class RenderSystem;

		struct Material {
			Utilities::Vector3 ambient = Utilities::Vector3(1.0);
			Utilities::Vector3 diffuse = Utilities::Vector3(1.0);
			Utilities::Vector3 specular = Utilities::Vector3(1.0);
			float shininess = 1.0;
		};

		class Object {
			friend class RenderSystem;
			public:
				Object(const std::vector<Utilities::Vertex>& vertices, const Utilities::Color& color = Utilities::Color(255, 255, 255, 255), const Visuals::Texture& textureData = Visuals::Texture::empty, const Visuals::Texture& normalMapData = Visuals::Texture::empty);
				Object(const std::vector<Utilities::Vertex>& vertices, const std::vector<uint32_t>& indices, const Utilities::Color& color = Utilities::Color(255, 255, 255, 255), const Visuals::Texture& textureData = Visuals::Texture::empty, const Visuals::Texture& normalMapData = Visuals::Texture::empty);
				Object(const Object& instance, const Utilities::Color& color = Utilities::Color(255, 255, 255, 255), const Visuals::Texture& textureData = Visuals::Texture::empty, const Visuals::Texture& normalMapData = Visuals::Texture::empty);
				void SetTexture(const Visuals::Texture& textureData);
				void SetNormalMap(const Visuals::Texture& normalData);
				void FreeTexture();
				void FreeNormalMap();
				Utilities::Color color;
				Material material;
				void AddShader(Render::ShaderProgram program);
				void RemoveShader(Render::ShaderType type);
			private:
				float* vertices;
				size_t vertexCount;
				uint32_t* indices;
				size_t indexCount;
				std::map<Render::ShaderType, Render::ShaderProgram> shaders;
				Visuals::Texture texture = Visuals::Texture::empty, normalMap = Visuals::Texture::empty;
		};
	}
}
#endif
