#pragma once
#ifdef StevEngine_RENDERER_GL
#include "Shader.hpp"
#include <vector>
#include <glad/gl.h>
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float2.hpp"

namespace StevEngine {
    namespace Render {

        class System;
        class ShaderProgram {
            friend class System;
            public:
                ShaderProgram(ShaderType shaderType);
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
            private:
                unsigned int location;
                ShaderType shaderType;
                bool modified;
        };
    }
}
#endif
