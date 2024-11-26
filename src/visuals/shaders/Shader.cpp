#ifdef StevEngine_RENDERER_GL
#include "Shader.hpp"
#include "main/Log.hpp"
#include "glad/gl.h"
#include <string>

namespace StevEngine::Renderer {
	//Definition sources
	const char* fragmentDefinitionSource =
		#include "visuals/shaders/definitions.frag"
	;
	const char* vertexDefinitionSource =
		#include "visuals/shaders/definitions.vert"
	;
	Shader::Shader(const char* source, ShaderType shaderType) : shaderType(shaderType), source(source) {
		//Create
		location = glCreateShader(shaderType == VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
		//Add definitions
		const char* definitions = shaderType == VERTEX ? vertexDefinitionSource : fragmentDefinitionSource;
		std::string src(source);
		int loc = src.find("\n", src.find_first_of("#version"));
		src = src.substr(0, loc) + std::string(definitions) + src.substr(loc);
		const char* newSource = src.c_str();
		//Compile
		glShaderSource(location, 1, &newSource, NULL);
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
