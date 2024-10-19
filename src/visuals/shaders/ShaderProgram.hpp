#pragma once
#ifdef StevEngine_RENDERER_GL
#include "Shader.hpp"
#include "utilities/Vector3.hpp"
#include "utilities/Vector2.hpp"
#include "utilities/Color.hpp"

#include <map>
#include <cstdint>
#include <glad/gl.h>
#include <yaml-cpp/yaml.h>
#include "glm/mat4x4.hpp"

namespace StevEngine::Renderer {
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
			void SetShaderUniform(const char* name, Utilities::Color value) const;
			void SetShaderUniform(const char* name, Utilities::Vector3 value) const;
			void SetShaderUniform(const char* name, Utilities::Vector2 value) const;
			void SetShaderUniform(const char* name, bool value) const;
			void SetShaderUniform(const char* name, int value) const;
			void SetShaderUniform(const char* name, float value) const;
			void SetShaderUniform(const char* name, double value) const;
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
#endif
