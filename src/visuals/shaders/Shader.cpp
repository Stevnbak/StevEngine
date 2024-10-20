#ifdef StevEngine_RENDERER_GL
#include "Shader.hpp"
#include "visuals/renderer/RenderSystem.hpp"
#include "visuals/renderer/Object.hpp"
#include "main/Log.hpp"
#include "glad/gl.h"
#include <string>

namespace StevEngine::Renderer {
	Shader::Shader(const char* source, ShaderType shaderType) : shaderType(shaderType), source(source) {
		//Create
		location = glCreateShader(shaderType == VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
		//Compile
		glShaderSource(location, 1, &source, NULL);
		glCompileShader(location);
		int  success;
		char infoLog[512];
		glGetShaderiv(location, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(location, 512, NULL, infoLog);
			Log::Error("Shader " + std::to_string(shaderType) + " failed to compile!\n" + std::string(infoLog));
		}
	}
}
#endif
