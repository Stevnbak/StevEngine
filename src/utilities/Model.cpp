#ifdef StevEngine_MODELS
#include "Model.hpp"
#include "assimp/Importer.hpp"
#include "main/Log.hpp"
#include "utilities/Vector3.hpp"
#include "utilities/Range3.hpp"

#include <cstddef>

#include "assimp/mesh.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

namespace StevEngine {
	namespace Utilities {
		Assimp::Importer importer;
		Model::Model(Resources::Resource file) : path(file.path) {
			//Load scene
			std::string fileType = file.path.substr(file.path.find_last_of('.') + 1);
			assimpScene = importer.ReadFileFromMemory(file.GetRawData(), file.GetSize(), aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_GenNormals | aiProcess_GenBoundingBoxes | aiProcess_JoinIdenticalVertices | aiProcess_GenUVCoords | aiProcess_FlipUVs, fileType.c_str());
			if(!assimpScene || assimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimpScene->mRootNode) {
				Log::Error(std::format("Couldn't load {}: {}", file.path, importer.GetErrorString()), true);
				assimpScene = NULL;
				return;
			}
			//Read meshes
			meshes.reserve(assimpScene->mNumMeshes);
			for(int i = 0; i < assimpScene->mNumMeshes; i++) {
				aiMesh* assimpMesh = assimpScene->mMeshes[i];
				std::vector<Vertex> vertices;
				std::vector<uint32_t> indices;
				//Bounding box
				Range3 bounds = Range3(assimpMesh->mAABB.mMin, assimpMesh->mAABB.mMax);
				Vector3 center = bounds.GetCenter();
				//Vertices
				vertices.reserve(assimpMesh->mNumVertices);
				for(int v = 0; v < assimpMesh->mNumVertices; v++) {
					Vector3 coords = ((Vector3)assimpMesh->mVertices[v]) - center;
					Vector3 tex = assimpMesh->HasTextureCoords(v) ? assimpMesh->mTextureCoords[0][v] : aiVector3D();
					Vector3 normal = ((Vector3)assimpMesh->mNormals[v]);
					Vector3 tangent = Vector3(1.0);
					//Calculate tangent
					if(assimpMesh->HasTangentsAndBitangents()) {
						Vector3 tangent = ((Vector3)assimpMesh->mTangents[v]);
					}
					vertices.emplace_back(coords, (Vector2)tex, normal, tangent);
				}
				//Indices
				for(int i = 0; i < assimpMesh->mNumFaces; i++)
				{
					aiFace face = assimpMesh->mFaces[i];
					indices.reserve(face.mNumIndices);
					for(uint32_t j = 0; j < face.mNumIndices; j++)
						indices.emplace_back(face.mIndices[j]);
				}
				meshes.emplace_back(vertices, indices);
			}
		}

		std::vector<Mesh> Model::GetMeshes() const {
			return meshes;
		}
	}
}
#endif
