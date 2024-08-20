#include "Shader.hpp"
#include "visuals/render/System.hpp"
#include "main/Log.hpp"
#include "visuals/render/Object.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace StevEngine::Render {
    Shader::Shader(const char* source, GLenum shaderType) {
        location = glCreateShader(shaderType);
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
    void Shader::FreeShader() {
        glDeleteShader(location);
        delete this;
    }
    void ReLinkShaderProgram(unsigned int shaderProgram) {
        glLinkProgram(shaderProgram);
        GLint success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success) {
            char infoLog[512];
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            Log::Error("Shader program failed to compile!\n" + std::string(infoLog));
        }
    }
    void Shader::AddToProgram(unsigned int shaderProgram) {
        glAttachShader(shaderProgram, location);
        ReLinkShaderProgram(shaderProgram);
    }
    void Shader::RemoveFromProgram(unsigned int shaderProgram) {
        glDetachShader(shaderProgram, location);
        ReLinkShaderProgram(shaderProgram);
    }

    //Set uniforms
    void System::SetShaderUniform(const char* name, glm::mat4 value) {
        glUniformMatrix4fv(glGetUniformLocation(defaultShaderProgram, name), 1, GL_FALSE, glm::value_ptr(value));
    }
    void System::SetShaderUniform(const char* name, glm::vec4 value) {
        glUniform4fv(glGetUniformLocation(defaultShaderProgram, name), 1, glm::value_ptr(value));
    }
    void System::SetShaderUniform(const char* name, glm::vec3 value) {
        glUniform3fv(glGetUniformLocation(defaultShaderProgram, name), 1, glm::value_ptr(value));
    }
    void System::SetShaderUniform(const char* name, glm::vec2 value) {
        glUniform2fv(glGetUniformLocation(defaultShaderProgram, name), 1, glm::value_ptr(value));
    }
    void System::SetShaderUniform(const char* name, bool value) {
        glUniform1i(glGetUniformLocation(defaultShaderProgram, name), value);
    }
    void System::SetShaderUniform(const char* name, int value) {
        glUniform1i(glGetUniformLocation(defaultShaderProgram, name), value);
    }
    void System::SetShaderUniform(const char* name, float value) {
        glUniform1f(glGetUniformLocation(defaultShaderProgram, name), value);
    }
}
