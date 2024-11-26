#pragma once
#ifdef StevEngine_RENDERER_GL
#include <cstdint>

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
