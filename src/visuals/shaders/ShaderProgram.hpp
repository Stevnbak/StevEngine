#pragma once
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
			friend class RenderSystem;
			public:
				ShaderProgram(ShaderType shaderType);
				ShaderProgram(YAML::Node node);
				ShaderProgram() {};
				//Set shaders
				unsigned int AddShader(Shader shader);
				void RemoveShader(unsigned int location);
				void RelinkProgram();
				//Set shader uniforms
				void SetShaderUniform(const char* name, glm::mat4 value);
				void SetShaderUniform(const char* name, glm::vec4 value);
				void SetShaderUniform(const char* name, glm::vec3 value);
				void SetShaderUniform(const char* name, glm::vec2 value);
				void SetShaderUniform(const char* name, bool value);
				void SetShaderUniform(const char* name, int value);
				void SetShaderUniform(const char* name, float value);
				//Shader type
				ShaderType GetType() { return shaderType; };
				//Export
				YAML::Node Export() const;
			private:
				unsigned int location;
				ShaderType shaderType;
				bool modified;
				std::map<unsigned int, Shader> shaders;
		};
	}
}
#endif
