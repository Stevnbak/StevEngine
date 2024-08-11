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
            for(int i = 0; i < assimpScene->mNumMeshes; i++) {
                aiMesh* assimpMesh = assimpScene->mMeshes[i];
                std::vector<Vertex> vertices;
                std::vector<unsigned int> indices;
                //Bounding box
                Utilities::Range3 bounds = Utilities::Range3(assimpMesh->mAABB.mMin, assimpMesh->mAABB.mMax);
                Utilities::Vector3 center = bounds.GetCenter();
                //Vertices
                for(int v = 0; v < assimpMesh->mNumVertices; v++) {
                    Utilities::Vector3 coords = ((Utilities::Vector3)assimpMesh->mVertices[v]) - center;
                    Utilities::Vector3 tex = assimpMesh->HasTextureCoords(v) ? assimpMesh->mTextureCoords[0][v] : aiVector3D();
                    Utilities::Vector3 normal = ((Utilities::Vector3)assimpMesh->mNormals[v]);
                    vertices.push_back(Vertex(coords, normal, (Utilities::Vector2)tex));
                }
                //Indices
                for(unsigned int i = 0; i < assimpMesh->mNumFaces; i++)
                {
                    aiFace face = assimpMesh->mFaces[i];
                    for(unsigned int j = 0; j < face.mNumIndices; j++)
                        indices.push_back(face.mIndices[j]);
                }
                meshes.push_back(Mesh(vertices, indices));
            }
        }

        std::vector<Mesh> Model::GetMeshes() {
            return meshes;
        }
    }
}
#endif
