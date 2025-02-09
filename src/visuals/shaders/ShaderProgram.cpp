#ifdef StevEngine_RENDERER_GL
#include "ShaderProgram.hpp"
#include "Shader.hpp"
#include "main/Log.hpp"

#include <string>

namespace StevEngine::Renderer {
	const char* vertexMainSource =
		#include "visuals/shaders/main.vert"
	;
	const char* fragmentMainSource =
		#include "visuals/shaders/main.frag"
	;
	const char* lightSource =
		#include "visuals/shaders/lights.frag"
	;
	ShaderProgram::ShaderProgram(ShaderType shaderType) : shaderType(shaderType), modified(true), location(glCreateProgram()) {
		glProgramParameteri(location, GL_PROGRAM_SEPARABLE, GL_TRUE);
		//Add main shaders
		if(shaderType == VERTEX) {
			AddShader(Shader(vertexMainSource, shaderType));
		} else {
			AddShader(Shader(fragmentMainSource, shaderType));
			AddShader(Shader(lightSource, shaderType));
		}
	}

	uint32_t ShaderProgram::AddShader(Shader shader) {
		glAttachShader(location, shader.location);
		shaders.insert({shader.location, shader});
		modified = true;
		return shader.location;
	}

	void ShaderProgram::RemoveShader(uint32_t location) {
		glDetachShader(this->location, location);
		shaders.erase(location);
		glDeleteShader(location);
		modified = true;
	}

	void ShaderProgram::RelinkProgram() {
		//Link program
		glLinkProgram(location);
		GLint success;
		glGetProgramiv(location, GL_LINK_STATUS, &success);
		if(!success) {
			char infoLog[512];
			glGetProgramInfoLog(location, 512, NULL, infoLog);
			Log::Error("Shader program failed to compile!\n" + std::string(infoLog));
		}
		modified = false;
	}

	Stream ShaderProgram::Export(StreamType type) const {
		Stream stream(type);
		stream << (uint8_t)shaderType << (uint)shaders.size();

		for(auto&[loc, shader] : shaders) stream << (std::string(shader.source));

		return stream;
	}
	ShaderProgram::ShaderProgram(Stream& stream) : shaderType((ShaderType)stream.Read<uint8_t>()), modified(true), location(glCreateProgram()) {
		glProgramParameteri(location, GL_PROGRAM_SEPARABLE, GL_TRUE);
		uint size = stream.Read<uint>();
		for(int i = 0; i < size; i++)
			AddShader(Shader(stream.Read<std::string>().c_str(), shaderType));
	}

	//Set uniforms
	void ShaderProgram::SetShaderUniform(const char* name, Utilities::Matrix4 value) const {
		glProgramUniformMatrix4fv(location, glGetUniformLocation(location, name), 1, GL_FALSE, value.data());
	}

	void ShaderProgram::SetShaderUniform(const char* name, Utilities::Color value) const {
		glProgramUniform4fv(location, glGetUniformLocation(location, name), 1, value.data());
	}
	void ShaderProgram::SetShaderUniform(const char* name, Utilities::Vector3 value) const {
		glProgramUniform3fv(location, glGetUniformLocation(location, name), 1, value.data());
	}
	void ShaderProgram::SetShaderUniform(const char* name, Utilities::Vector2 value) const {
		glProgramUniform2fv(location, glGetUniformLocation(location, name), 1, value.data());
	}
	void ShaderProgram::SetShaderUniform(const char* name, bool value) const {
		glProgramUniform1i(location, glGetUniformLocation(location, name), value);
	}
	void ShaderProgram::SetShaderUniform(const char* name, int value) const {
		glProgramUniform1i(location, glGetUniformLocation(location, name), value);
	}
	void ShaderProgram::SetShaderUniform(const char* name, float value) const {
		glProgramUniform1f(location, glGetUniformLocation(location, name), value);
	}
	void ShaderProgram::SetShaderUniform(const char* name, double value) const {
		glProgramUniform1d(location, glGetUniformLocation(location, name), value);
	}
}
#endif
