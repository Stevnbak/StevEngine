#pragma once
#ifdef StevEngine_RENDERER_GL
#include <vector>
#include <cstdint>
#include <SDL.h>
#include <glad/gl.h>

#include "utilities/Vertex.hpp"
#include "utilities/Matrix4.hpp"
#include "visuals/Material.hpp"
#include "visuals/shaders/ShaderProgram.hpp"

namespace StevEngine::Renderer {
	/**
	 * @brief Base interface for renderable objects
	 *
	 * Abstract base class that defines the interface for any object
	 * that can be rendered in the scene.
	 */
	class CustomObject {
		public:
			/**
			 * @brief Draw the object with transform
			 * @param transform World transform matrix to apply
			 */
			virtual void Draw(Utilities::Matrix4 transform) const = 0;
	};

	/**
	 * @brief Standard renderable mesh object
	 *
	 * Manages vertex data, indices, and materials for rendering 3D meshes.
	 * Handles OpenGL buffer management and shader program binding.
	 */
	class Object : public CustomObject {
		public:
			/**
			 * @brief Create object from vertices
			 * @param vertices Array of vertex data
			 * @param material Material to render with
			 */
			Object(const std::vector<Utilities::Vertex>& vertices, const Visuals::Material& material);

			/**
			 * @brief Create object from vertices and indices
			 * @param vertices Array of vertex data
			 * @param indices Array of vertex indices
			 * @param material Material to render with
			 */
			Object(const std::vector<Utilities::Vertex>& vertices, const std::vector<uint32_t>& indices, const Visuals::Material& material);

			/**
			 * @brief Copy constructor
			 * @param instance Object to copy
			 */
			Object(const Object& instance);

			/** @brief Material used for rendering */
			Visuals::Material material;

			/**
			 * @brief Add shader program
			 * @param program Shader program to add
			 */
			void AddShader(Renderer::ShaderProgram program);

			/**
			 * @brief Remove shader of specified type
			 * @param type Type of shader to remove
			 */
			void RemoveShader(Renderer::ShaderType type);

			/**
			 * @brief Draw object with transform
			 * @param transform World transform matrix
			 */
			void Draw(Utilities::Matrix4 transform) const;

		private:
			float* vertices;	  ///< Vertex data array
			size_t vertexCount;  ///< Number of vertices
			uint32_t* indices;	///< Index data array
			size_t indexCount;   ///< Number of indices
			std::map<Renderer::ShaderType, Renderer::ShaderProgram> shaders;  ///< Shader programs by type
	};
}
#endif
