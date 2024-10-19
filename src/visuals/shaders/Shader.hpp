#pragma once
#ifdef StevEngine_RENDERER_GL
#include <string>
#include "glm/mat4x4.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace StevEngine::Renderer {
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
			uint32_t location;
			const char* source;
	};
}
#endif
