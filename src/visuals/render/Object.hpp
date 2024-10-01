#pragma once
#ifdef StevEngine_RENDERER_GL
#include <vector>
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
				Object(const std::vector<Utilities::Vertex>& vertices, Utilities::Color color = Utilities::Color(255, 255, 255, 255), Visuals::Texture textureData = Visuals::Texture::empty);
				Object(const std::vector<Utilities::Vertex>& vertices, const std::vector<unsigned int>& indices, Utilities::Color color = Utilities::Color(255, 255, 255, 255), Visuals::Texture textureData = Visuals::Texture::empty);
				Object(const Object& instance, Utilities::Color color = Utilities::Color(255, 255, 255, 255), Visuals::Texture textureData = Visuals::Texture::empty);
				void SetTexture(Visuals::Texture textureData);
				void FreeTexture();
				Utilities::Color color;
				Material material;
				void AddShader(Render::ShaderProgram program);
				void RemoveShader(Render::ShaderType type);
			private:
				std::vector<float> vertices;
				std::vector<unsigned int> indices;
				std::map<Render::ShaderType, Render::ShaderProgram> shaders;
				GLuint texture;
				bool textured;
		};
	}
}
#endif
