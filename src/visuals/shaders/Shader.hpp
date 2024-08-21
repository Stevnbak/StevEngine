#pragma once
#ifdef StevEngine_RENDERER_GL
#include <string>
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float2.hpp"

namespace StevEngine {
    namespace Render {
        class ShaderProgram;

        enum ShaderType {
            VERTEX,
            FRAGMENT
        };

        class Shader {
            friend class ShaderProgram;
            public:
                Shader(const char* source, ShaderType shaderType);
                const ShaderType shaderType;
            private:
                unsigned int location;
        };
    }
}
#endif
