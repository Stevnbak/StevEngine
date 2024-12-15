#pragma once
#include "utilities/Matrix4.hpp"
#ifdef StevEngine_RENDERER_GL
#include "Shader.hpp"
#include "utilities/Vector3.hpp"
#include "utilities/Vector2.hpp"
#include "utilities/Color.hpp"

#include <map>
#include <cstdint>
#include <glad/gl.h>
#include <yaml-cpp/yaml.h>

namespace StevEngine::Renderer {
	/**
	 * @brief Compiled and linked shader program
	 *
	 * Manages multiple shader stages combined into a complete program.
	 * Handles uniform updates and shader program state.
	 */
	class ShaderProgram {
		public:
			/**
			 * @brief Create shader program
			 * @param shaderType Type of shaders in program
			 */
			ShaderProgram(ShaderType shaderType);

			/**
			 * @brief Create from serialized data
			 * @param node YAML node with program data
			 */
			ShaderProgram(YAML::Node node);

			/** @brief Create empty program */
			ShaderProgram() {};

			/**
			 * @brief Add shader stage to program
			 * @param shader Shader to add
			 * @return OpenGL shader ID
			 */
			uint32_t AddShader(Shader shader);

			/**
			 * @brief Remove shader from program
			 * @param location OpenGL shader ID to remove
			 */
			void RemoveShader(uint32_t location);

			/** @brief Relink program after shader changes */
			void RelinkProgram();

			// Uniform setters
			/**
			 * @brief Set matrix uniform
			 * @param name Uniform name in shader
			 * @param value Matrix value
			 */
			void SetShaderUniform(const char* name, Utilities::Matrix4 value) const;

			/**
			 * @brief Set color uniform
			 * @param name Uniform name in shader
			 * @param value Color value
			 */
			void SetShaderUniform(const char* name, Utilities::Color value) const;

			/**
			 * @brief Set vector3 uniform
			 * @param name Uniform name in shader
			 * @param value Vector value
			 */
			void SetShaderUniform(const char* name, Utilities::Vector3 value) const;

			/**
			 * @brief Set vector2 uniform
			 * @param name Uniform name in shader
			 * @param value Vector value
			 */
			void SetShaderUniform(const char* name, Utilities::Vector2 value) const;

			/**
			 * @brief Set boolean uniform
			 * @param name Uniform name in shader
			 * @param value Boolean value
			 */
			void SetShaderUniform(const char* name, bool value) const;

			/**
			 * @brief Set integer uniform
			 * @param name Uniform name in shader
			 * @param value Integer value
			 */
			void SetShaderUniform(const char* name, int value) const;

			/**
			 * @brief Set float uniform
			 * @param name Uniform name in shader
			 * @param value Float value
			 */
			void SetShaderUniform(const char* name, float value) const;

			/**
			 * @brief Set double uniform
			 * @param name Uniform name in shader
			 * @param value Double value
			 */
			void SetShaderUniform(const char* name, double value) const;

			/**
			 * @brief Get OpenGL program ID
			 * @return Program location in OpenGL
			 */
			uint32_t GetLocation() const { return location; }

			/**
			 * @brief Check if program needs relinking
			 * @return true if shaders were modified
			 */
			bool IsModified() const { return modified; }

			/**
			 * @brief Get program shader type
			 * @return Type of shaders in program
			 */
			ShaderType GetType() const { return shaderType; };

			/**
			 * @brief Serialize program to YAML
			 * @return YAML node with program data
			 */
			YAML::Node Export() const;

		private:
			uint32_t location;	  ///< OpenGL program ID
			ShaderType shaderType;  ///< Type of shaders in program
			bool modified;		  ///< Whether program needs relinking
			std::map<uint32_t, Shader> shaders;  ///< Shaders by OpenGL ID
	};
}
#endif
