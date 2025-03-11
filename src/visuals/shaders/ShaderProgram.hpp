#pragma once
#ifdef StevEngine_RENDERER_GL
#include "Shader.hpp"
#include "utilities/Vector3.hpp"
#include "utilities/Vector2.hpp"
#include "utilities/Color.hpp"
#include "utilities/Matrix4.hpp"
#include "utilities/Stream.hpp"
#include "visuals/Texture.hpp"

#include <map>
#include <cstdint>
#include <glad/gl.h>

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
			 * @param useDefaultShaders Add default engine shaders to program
			 */
			ShaderProgram(ShaderType shaderType, bool useDefaultShaders = true);

			/**
			 * @brief Create shader program
			 *
			 * The shader type will be inferred from the first shader in vector, any shader of different type will be ignored.
			 *
			 * @param shaders Vector of shaders to initialize program with.
			 * @param useDefaultShaders Add default engine shaders to program
			 */
			ShaderProgram(const std::vector<Shader>& shaders, bool useDefaultShaders = true);

			/**
			 * @brief Create from serialized data
			 * @param stream Stream containing serialized component data
			 */
			ShaderProgram(Utilities::Stream& stream);

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

			/** @brief Deletes program and all shaders from OpenGL */
			void DeleteProgram();

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
			void SetShaderUniform(const char* name, int32_t value) const;

			/**
			 * @brief Set unsigned integer uniform
			 * @param name Uniform name in shader
			 * @param value Unsigned integer value
			 */
			void SetShaderUniform(const char* name, uint32_t value) const;

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
			 * @brief Serialize shader program to a stream
			 * @param type Type of stream to export to
			 * @return Serialized stream
			 */
			Utilities::Stream Export(Utilities::StreamType type) const;

		protected:
			uint32_t location;	  ///< OpenGL program ID
			ShaderType shaderType;  ///< Type of shaders in program
			bool modified;		  ///< Whether program needs relinking
			std::map<uint32_t, Shader> shaders;  ///< Shaders by OpenGL ID
	};

	class ComputeShader : public ShaderProgram {
		public:
			/**
			 * @brief Create compute shader program
			 *, with a single shader from source.
			 *
			 * @param source Source for the default shader to create
			 */
			ComputeShader(const char* source);

			/**
			 * @brief Create compute shader program
			 *
			 * @param shaders Vector of shaders to initialize program with.
			 */
			ComputeShader(const std::vector<Shader>& shaders) : ShaderProgram(shaders) {};

			/**
			 * @brief Create from serialized data
			 * @param stream Stream containing serialized component data
			 */
			ComputeShader(Utilities::Stream& stream) : ShaderProgram(stream) {};

			/**
			 * @brief Run the compute shader
			 *
			 * Group size is a single pixel of the output texture;
			 *
			 * @param output Compute texture to save output of shader to
			 */
			void Run(const Visuals::ComputeTexture& output) const { return Run(output, output.width, output.height, 1); }
			/**
			 * @brief Run the compute shader
			 * @param output Compute texture to save output of shader to
			 * @param groupsX Number of groups to use on the X-axis
			 * @param groupsY Number of groups to use on the Y-axis
			 * @param groupsZ Number of groups to use on the Z-axis
			 */
			void Run(const Visuals::ComputeTexture& output, uint32_t groupsX, uint32_t groupsY, uint32_t groupsZ = 1) const;
			/**
			 * @brief Run the compute shader
			 * @param groupsX Number of groups to use on the X-axis
			 * @param groupsY Number of groups to use on the Y-axis
			 * @param groupsZ Number of groups to use on the Z-axis
			 */
			void Run(uint32_t groupsX, uint32_t groupsY, uint32_t groupsZ = 1) const;

	};
}
#endif
