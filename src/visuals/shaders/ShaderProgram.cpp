
#ifdef StevEngine_RENDERER_GL
#include "ShaderProgram.hpp"
#include "visuals/shaders/Shader.hpp"
#include "main/Log.hpp"
#include "utilities/ID.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace StevEngine::Render {
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

    unsigned int ShaderProgram::AddShader(Shader shader) {
        glAttachShader(location, shader.location);
        modified = true;
        return shader.location;
    }

    void ShaderProgram::RemoveShader(unsigned int location) {
        glDetachShader(this->location, location);
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

    //Set uniforms
    void ShaderProgram::SetShaderUniform(const char* name, glm::mat4 value) {
        glProgramUniformMatrix4fv(location, glGetUniformLocation(location, name), 1, GL_FALSE, glm::value_ptr(value));
    }
    void ShaderProgram::SetShaderUniform(const char* name, glm::vec4 value) {
        glProgramUniform4fv(location, glGetUniformLocation(location, name), 1, glm::value_ptr(value));
    }
    void ShaderProgram::SetShaderUniform(const char* name, glm::vec3 value) {
        glProgramUniform3fv(location, glGetUniformLocation(location, name), 1, glm::value_ptr(value));
    }
    void ShaderProgram::SetShaderUniform(const char* name, glm::vec2 value) {
        glProgramUniform2fv(location, glGetUniformLocation(location, name), 1, glm::value_ptr(value));
    }
    void ShaderProgram::SetShaderUniform(const char* name, bool value) {
        glProgramUniform1i(location, glGetUniformLocation(location, name), value);
    }
    void ShaderProgram::SetShaderUniform(const char* name, int value) {
        glProgramUniform1i(location, glGetUniformLocation(location, name), value);
    }
    void ShaderProgram::SetShaderUniform(const char* name, float value) {
        glProgramUniform1f(location, glGetUniformLocation(location, name), value);
    }
}
#endif
