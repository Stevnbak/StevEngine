#ifdef StevEngine_RENDERER_GL
#include "Shader.hpp"
#include "main/Log.hpp"
#include "glad/gl.h"
#include <string>

namespace StevEngine::Renderer {
	GLenum OpenGLShaderType(const ShaderType& type) {
		switch(type) {
			case VERTEX: return GL_VERTEX_SHADER;
			case FRAGMENT: return GL_FRAGMENT_SHADER;
			case COMPUTE: return GL_COMPUTE_SHADER;
			default: return -1;
		}
	}

	//Definition sources
	const char* fragmentDefinitionSource =
		#include "visuals/shaders/definitions.frag"
	;
	const char* vertexDefinitionSource =
		#include "visuals/shaders/definitions.vert"
	;
	bool AddDefinitions(const ShaderType& type, std::string& src) {
		const char* definitions;
		switch(type) {
			case VERTEX:
				definitions = vertexDefinitionSource;
				break;
			case FRAGMENT:
		 		definitions = fragmentDefinitionSource;
				break;
			default:
				return false;
		};
		int loc = src.find("\n", src.find_first_of("#version"));
		src = src.substr(0, loc) + std::string(definitions) + src.substr(loc);
		return true;
	}

	//Shader constructor
	Shader::Shader(const char* source, ShaderType shaderType, bool useDefaultDefinitions) : shaderType(shaderType), source(source) {
		//Create
		location = glCreateShader(OpenGLShaderType(shaderType));
		//Add definitions
		std::string src(source);
		const char* newSource;
		if(useDefaultDefinitions && AddDefinitions(shaderType, src)) newSource = src.c_str();
		else newSource = source;
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
