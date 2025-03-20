#pragma once
#include "utilities/Range3.hpp"
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

	enum RenderType {
		SOLID,
		WIREFRAME
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
			Object(const std::vector<Utilities::Vertex>& vertices, const Visuals::Material& material, RenderType renderType = SOLID);

			/**
			 * @brief Create object from vertices and indices
			 * @param vertices Array of vertex data
			 * @param indices Array of vertex indices
			 * @param material Material to render with
			 */
			Object(const std::vector<Utilities::Vertex>& vertices, const std::vector<uint32_t>& indices, const Visuals::Material& material, RenderType renderType = SOLID);

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

			/**
			 * @brief Update GPU buffers with object vertex and index data
			 */
			void UpdateBuffers() const;

			/**
			 * @brief Update Fragment shader with material data
			 */
			void UpdateShaderMaterial() const;

			/**
			 * @brief Get the number of indices in object
			 * @return Number of indices
			 */
			uint32_t GetIndexCount() const { return indexCount; }
			/**
			 * @brief Get the number of indices in object
			 * @return Number of indices
			 */
			uint32_t GetVertexCount() const { return vertexCount; }
			/**
			 * @brief Get bouning box of object
			 * @return Bounding box
			 */
			Utilities::Range3 GetBoundingBox() const { return boundingBox; }
			/**
			 * @brief Get the type of rendering used by this object
			 * @return Render type
			 */
			RenderType GetRenderType() const { return renderType; }
			/**
			 * @brief Set the type of rendering used by this object
			 * @return Render type
			 */
			void SetRenderType(RenderType type);

		private:
			float* vertices;	  ///< Vertex data array
			uint32_t vertexCount;  ///< Number of vertices
			uint32_t* indices;	///< Index data array
			uint32_t indexCount;   ///< Number of indices
			std::map<Renderer::ShaderType, Renderer::ShaderProgram> shaders;  ///< Shader programs by type
			Utilities::Range3 boundingBox; ///< Bounding box
			RenderType renderType; ///> Render type
	};
}
#endif
