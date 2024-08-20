#pragma once
#ifdef StevEngine_RENDERER_GL
#include <glad/glad.h>
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float2.hpp"

namespace StevEngine {
    namespace Render {
        class Shader {
            public:
                Shader() {}
                Shader(const char* source, GLenum shaderType);
                void FreeShader();
                void AddToProgram(unsigned int shaderProgram);
                void RemoveFromProgram(unsigned int shaderProgram);
            private:
                unsigned int location;
        };
    }
}
#endif
