#pragma once
#ifdef StevEngine_RENDERER_GL
#include "main/Component.hpp"
#include "utilities/Model.hpp"
#include "utilities/Quaternion.hpp"
#include "utilities/Vector3.hpp"
#include "visuals/renderer/Object.hpp"
#include "visuals/Material.hpp"
#include <cstddef>

#define MODEL_RENDERER_TYPE "ModelRenderer"

namespace StevEngine::Visuals {
	/**
	 * @brief Component for rendering 3D models
	 *
	 * Handles rendering of 3D mesh data with materials and shaders.
	 * Supports multiple meshes per model with individual materials.
	 */
	class ModelRenderer : public Component {
		public:
			/**
			 * @brief Create model renderer
			 * @param model 3D model to render
			 * @param material Default material if model has none
			 * @param position Local position offset
			 * @param rotation Local rotation offset
			 * @param scale Local scale modifier
			 */
			ModelRenderer(const Utilities::Model& model,
				 		  const Material& material = Material(),
						  Utilities::Vector3 position = Utilities::Vector3(),
						  Utilities::Quaternion rotation = Utilities::Quaternion(),
						  Utilities::Vector3 scale = Utilities::Vector3(1)
			);

			/**
			 * @brief Create model renderer from text serialized data
			 * @param stream Stream containing serialized component data
			 */
			ModelRenderer(Utilities::Stream& stream);

			/**
			 * @brief Get component type
			 * @return Type identifier string
			 */
			std::string GetType() const { return MODEL_RENDERER_TYPE; }

			/**
			 * @brief Serialize component to a stream
			 * @param type Type of stream to export to
			 * @return Serialized stream
			 */
			Utilities::Stream Export(Utilities::StreamType type) const;

			/** @brief Local position offset */
			Utilities::Vector3 position = Utilities::Vector3();
			/** @brief Local rotation offset */
			Utilities::Quaternion rotation = Utilities::Quaternion();
			/** @brief Local scale modifier */
			Utilities::Vector3 scale = Utilities::Vector3(1);

			/**
			 * @brief Get number of meshes in model
			 * @return Mesh count
			 */
			size_t MeshCount() const { return objects.size(); }

			/**
			 * @brief Get renderable object for mesh
			 * @param index Mesh index
			 * @return Reference to render object
			 */
			Renderer::Object& GetObject(uint index) { return objects.at(index); };

			/**
			 * @brief Get material for mesh
			 * @param index Mesh index
			 * @return Reference to material
			 */
			Visuals::Material& GetMaterials(uint index) { return GetObject(index).material; };

			/**
			 * @brief Add shader program
			 * @param program Shader to add
			 */
			void AddShader(Renderer::ShaderProgram program);

			/**
			 * @brief Remove shader of specified type
			 * @param type Type of shader to remove
			 */
			void RemoveShader(Renderer::ShaderType type);

			/**
			 * @brief Draw model with transform
			 * @param transform World transform matrix
			 */
			void Draw(const Utilities::Matrix4& transform);

			/**
			 * @brief Clean up component resources
			 */
			~ModelRenderer();

		private:
			Utilities::Model model;				 	///< Source 3D model data
			std::vector<Renderer::Object> objects;  ///< Renderable objects for each mesh
			std::map<Renderer::ShaderType, Renderer::ShaderProgram> shaders;  ///< Shader programs by type
	};

	/** @brief Register ModelRenderer as a component type */
	inline bool modelRenderer = CreateComponents::RegisterComponentType<ModelRenderer>(MODEL_RENDERER_TYPE);
}
#endif
