#pragma once
#include <cstdint>
#ifdef StevEngine_RENDERER_GL
#include "Shader.hpp"
#include <map>
#include <glad/gl.h>
#include <yaml-cpp/yaml.h>
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float2.hpp"

namespace StevEngine {
	namespace Render {
		class RenderSystem;

		class ShaderProgram {
			public:
				ShaderProgram(ShaderType shaderType);
				ShaderProgram(YAML::Node node);
				ShaderProgram() {};
				//Set shaders
				uint32_t AddShader(Shader shader);
				void RemoveShader(uint32_t location);
				void RelinkProgram();
				//Set shader uniforms
				void SetShaderUniform(const char* name, glm::mat4 value) const;
				void SetShaderUniform(const char* name, glm::vec4 value) const;
				void SetShaderUniform(const char* name, glm::vec3 value) const;
				void SetShaderUniform(const char* name, glm::vec2 value) const;
				void SetShaderUniform(const char* name, bool value) const;
				void SetShaderUniform(const char* name, int value) const;
				void SetShaderUniform(const char* name, float value) const;
				//Get info
				uint32_t GetLocation() const { return location; }
				bool IsModified() const { return modified; }
				ShaderType GetType() const { return shaderType; };
				//Export
				YAML::Node Export() const;
			private:
				uint32_t location;
				ShaderType shaderType;
				bool modified;
				std::map<uint32_t, Shader> shaders;
		};
	}
}
#endif
