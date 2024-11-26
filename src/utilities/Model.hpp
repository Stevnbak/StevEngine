#pragma once
#ifdef StevEngine_MODELS
#include "assimp/scene.h"

#include "main/ResourceManager.hpp"
#include "utilities/Vertex.hpp"
#include "visuals/Material.hpp"

#include <vector>

namespace StevEngine::Utilities {
	struct Mesh {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		#ifdef StevEngine_SHOW_WINDOW
		Visuals::Material material;
		Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Visuals::Material material)
			: vertices(vertices), indices(indices), material(material) {}
		#else
		Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
			: vertices(vertices), indices(indices) {}
		#endif
	};

	class Model {
		public:
			Model(const Resources::Resource& file);
			#ifdef StevEngine_SHOW_WINDOW
			bool hasMaterials;
			#endif
			const std::string path;
			std::vector<Mesh> GetMeshes() const;
		private:
			const aiScene* assimpScene;
			std::vector<Mesh> meshes;
	};
}
#endif
