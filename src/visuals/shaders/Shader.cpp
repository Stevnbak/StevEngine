#ifdef StevEngine_RENDERER_GL
#include "Shader.hpp"
#include "visuals/render/System.hpp"
#include "visuals/render/Object.hpp"
#include "main/Log.hpp"
#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>
#include <string>

namespace StevEngine::Render {
    Shader::Shader(const char* source, ShaderType shaderType) : shaderType(shaderType) {
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
            Log::Error("Shader failed to compile!\n" + std::string(infoLog));
        }
    }
}
#endif
