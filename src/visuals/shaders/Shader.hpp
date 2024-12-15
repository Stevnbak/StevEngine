#pragma once
#ifdef StevEngine_RENDERER_GL
#include <cstdint>
#include <string>

namespace StevEngine::Renderer {
	class ShaderProgram;

	enum ShaderType {
		VERTEX,
		FRAGMENT
	};

	/**
	 * @brief Single shader stage (vertex or fragment)
	 *
	 * Represents a single OpenGL shader stage with its source code.
	 * Handles compilation and resource management for individual shaders.
	 */
	class Shader {
		friend class ShaderProgram;
		public:
			/**
			 * @brief Create new shader
			 * @param source GLSL source code
			 * @param shaderType Type of shader (vertex/fragment)
			 */
			Shader(const char* source, ShaderType shaderType);

			/** @brief Type of this shader */
			const ShaderType shaderType;

		private:
			uint32_t location;	 ///< OpenGL shader object ID
			const char* source;	///< GLSL source code
	};
}
#endif
