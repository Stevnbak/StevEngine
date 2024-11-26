#pragma once
#ifdef StevEngine_RENDERER_GL
#include <vector>
#include <cstdint>
#include <SDL.h>
#include <glad/gl.h>

#include "utilities/Vertex.hpp"
#include "utilities/Matrix4.hpp"
#include "visuals/Material.hpp"
#include "visuals/shaders/ShaderProgram.hpp"

namespace StevEngine::Renderer {
	class CustomObject {
		public:
			virtual void Draw(Utilities::Matrix4 transform) const = 0;
	};

	class Object : public CustomObject {
		public:
			Object(const std::vector<Utilities::Vertex>& vertices, const Visuals::Material& material);
			Object(const std::vector<Utilities::Vertex>& vertices, const std::vector<uint32_t>& indices, const Visuals::Material& material);
			Object(const Object& instance);
			Visuals::Material material;
			void AddShader(Renderer::ShaderProgram program);
			void RemoveShader(Renderer::ShaderType type);
			void Draw(Utilities::Matrix4 transform) const;
		private:
			float* vertices;
			size_t vertexCount;
			uint32_t* indices;
			size_t indexCount;
			std::map<Renderer::ShaderType, Renderer::ShaderProgram> shaders;
	};
}
#endif
