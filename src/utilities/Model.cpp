#ifdef StevEngine_MODELS
#include "Model.hpp"
#include "main/ResourceManager.hpp"
#include "main/Log.hpp"
#include "visuals/Material.hpp"
#include "visuals/Texture.hpp"
#include "utilities/Vector3.hpp"
#include "utilities/Range3.hpp"
#include "utilities/Color.hpp"
#include <cstddef>

#include "assimp/Importer.hpp"
#include "assimp/mesh.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/types.h"
#include "assimp/material.h"

namespace StevEngine::Utilities {
	Assimp::Importer importer;
	Model::Model(const Resources::Resource& file) : path(file.path)
	{
		//Load scene
		std::string fileType = file.path.substr(file.path.find_last_of('.') + 1);
		assimpScene = importer.ReadFileFromMemory(file.GetRawData(), file.GetSize(), aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_GenNormals | aiProcess_GenBoundingBoxes | aiProcess_JoinIdenticalVertices | aiProcess_GenUVCoords | aiProcess_FlipUVs, fileType.c_str());
		if(!assimpScene || assimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimpScene->mRootNode) {
			Log::Error(std::format("Couldn't load {}: {}", file.path, importer.GetErrorString()), true);
			assimpScene = NULL;
			return;
		}
		#ifdef StevEngine_SHOW_WINDOW
		hasMaterials = assimpScene->HasMaterials();
		#endif

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
				Vector3 tex = assimpMesh->HasTextureCoords(0) ? assimpMesh->mTextureCoords[0][v] : aiVector3D();
				Vector3 normal = ((Vector3)assimpMesh->mNormals[v]);
				Vector3 tangent = assimpMesh->HasTangentsAndBitangents() ? ((Vector3)assimpMesh->mTangents[v]) : Vector3(1.0);
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
			//Materials
			#ifdef StevEngine_SHOW_WINDOW
				Visuals::Material material;
				if(hasMaterials) {
					aiMaterial* assimpMaterial = assimpScene->mMaterials[assimpMesh->mMaterialIndex];
					//Ambient
					aiColor3D ambient (1.f,1.f,1.f);
					assimpMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
					//Diffuse
					aiColor3D diffuse (1.f,1.f,1.f);
					assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
					//Specular
					aiColor3D specular (1.f,1.f,1.f);
					assimpMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specular);
					float shininessStrength = 1.0f;
					assimpMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, shininessStrength);
					//Shininess
					float shininess = 1.0f;
					assimpMaterial->Get(AI_MATKEY_SHININESS, shininess);
					//Albedo texture
					aiString albedoPath;
					bool hasAlbedo = assimpMaterial->GetTexture(aiTextureType_BASE_COLOR, 0, &albedoPath) == AI_SUCCESS;
					Visuals::Texture albedo = hasAlbedo ? Visuals::Texture(Resources::resourceManager.GetFile(std::string(albedoPath.C_Str()))) : Visuals::Texture::empty;
					//Normal texture
					aiString normalPath;
					bool hasNormal = assimpMaterial->GetTexture(aiTextureType_BASE_COLOR, 0, &normalPath) == AI_SUCCESS;
					Visuals::Texture normal = hasNormal ? Visuals::Texture(Resources::resourceManager.GetFile(std::string(normalPath.C_Str()))) : Visuals::Texture::empty;
					//Create material
					material = Visuals::Material(
						Utilities::Color(diffuse.r, diffuse.g, diffuse.b),
						Utilities::Vector3(ambient.r, ambient.g, ambient.b),
						Utilities::Vector3(diffuse.r, diffuse.g, diffuse.b),
						Utilities::Vector3(specular.r, specular.g, specular.b) * shininessStrength,
						shininess,
						albedo,
						normal
					);
				}
				//Push mesh
				meshes.emplace_back(vertices, indices, material);
			#else
				//Push mesh
				meshes.emplace_back(vertices, indices);
			#endif
		}
	}

	std::vector<Mesh> Model::GetMeshes() const {
		return meshes;
	}
}
#endif
