#pragma once
#ifdef StevEngine_RENDERER_GL
#include <cstdint>

namespace StevEngine::Renderer {
	class ShaderProgram;

	enum ShaderType {
		VERTEX,
		FRAGMENT,
		GEOMETRY,
		COMPUTE,
		TESS_CONTROL,
		TESS_EVALUATION
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
			 * @param useDefaultDefinitions Add default engine shader definitions to shader
			 */
			Shader(const char* source, ShaderType shaderType, bool useDefaultDefinitions = true);

			/** @brief Type of this shader */
			const ShaderType shaderType;

			/**
			 * @brief Get OpenGL shader ID
			 * @return OpenGL shader location
			 */
			uint32_t GetLocation() const { return location; };

		private:
			uint32_t location;	 ///< OpenGL shader object ID
			const char* source;	///< GLSL source code
	};
}
#endif
