#pragma once

#include "Vertex.hpp"

#ifdef StevEngine_MODELS
#include "assimp/scene.h"

#include "main/ResourceManager.hpp"
#include "utilities/Color.hpp"

#include <vector>

namespace StevEngine {
	namespace Utilities {

		struct Mesh {
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
				: vertices(vertices), indices(indices) {}
		};

		class Model {
			public:
				Model(Resources::Resource file);
				const std::string path;
				std::vector<Mesh> GetMeshes();
			private:
				const aiScene* assimpScene;
				std::vector<Mesh> meshes;
		};
	}
}
#endif
