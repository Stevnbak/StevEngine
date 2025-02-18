#pragma once
#ifdef StevEngine_MODELS
#include "assimp/scene.h"

#include "main/ResourceManager.hpp"
#include "utilities/Vertex.hpp"
#include "visuals/Material.hpp"

#include <vector>

namespace StevEngine::Utilities {
	/**
	 * @brief Single mesh data within a model
	 */
	struct Mesh {
		std::vector<Vertex> vertices;   ///< Mesh vertices
		std::vector<uint32_t> indices;  ///< Vertex indices

		#ifdef StevEngine_SHOW_WINDOW
		Visuals::Material material;  ///< Mesh material
		/**
		 * @brief Create mesh with material
		 * @param vertices Vertex array
		 * @param indices Index array
		 * @param material Material data
		 */
		Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Visuals::Material material)
		  : vertices(vertices), indices(indices), material(material) {}
		#else
		/**
		 * @brief Create mesh without material
		 * @param vertices Vertex array
		 * @param indices Index array
		 */
		Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
		  : vertices(vertices), indices(indices) {}
		#endif
	};

	/**
	 * @brief 3D model data container
	 *
	 * Loads and stores 3D model data using Assimp.
	 * Contains multiple meshes with vertices, indices and materials.
	 */
	class Model {
		public:
			/**
			 * @brief Load model from resource
			 * @param file Resource containing model data
			 */
			Model(const Resources::Resource& file);

			#ifdef StevEngine_SHOW_WINDOW
			/** @brief Whether model has material data */
			bool hasMaterials;
			#endif

			/** @brief Path to model file */
			const std::string path;

			/**
			 * @brief Get all meshes in model
			 * @return Vector of mesh data
			 */
			std::vector<Mesh> GetMeshes() const;

		private:
			const aiScene* assimpScene;  ///< Assimp scene data
			std::vector<Mesh> meshes;	 ///< Processed mesh data
	};
}
#endif
