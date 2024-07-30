#ifdef StevEngine_RENDERER_GL
#include "Model.hpp"
#include "assimp/mesh.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/vector2.h"
#include "assimp/vector3.h"
#include "main/Engine.hpp"
#include "main/Log.hpp"
#include "main/ResourceManager.hpp"
#include "visuals/render/Object.hpp"
#include <vector>

Assimp::Importer importer;

namespace StevEngine {
    namespace Visuals {
        const aiScene* LoadModel(Resources::Resource file) {
            const aiScene* scene = importer.ReadFileFromMemory(file.GetRawData(), file.GetSize(), aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_FlipUVs);
            if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                Log::Error(std::format("Couldn't load {}: {}", file.path, importer.GetErrorString()), true);
                scene = NULL;
            }
            return scene;
        }
        Render::Object CreateObject(const aiScene* scene) {
            if(!scene) {
                return Render::Object(std::vector<Render::Vertex>());
            }

            std::vector<Render::Vertex> vertices;
            std::vector<unsigned int> indices;
            for(int i = 0; i < scene->mNumMeshes; i++) {
                aiMesh* mesh = scene->mMeshes[i];
                //Vertices
                for(int v = 0; v < mesh->mNumVertices; v++) {
                    aiVector3D coords = mesh->mVertices[v];
                    aiVector3D tex = mesh->HasTextureCoords(v) ? mesh->mTextureCoords[0][v] : aiVector3D();
                    vertices.push_back(Render::Vertex(coords.x, coords.y, coords.z, tex.x, tex.y));
                }
                //Indices
                for(unsigned int i = 0; i < mesh->mNumFaces; i++)
                {
                    aiFace face = mesh->mFaces[i];
                    for(unsigned int j = 0; j < face.mNumIndices; j++)
                        indices.push_back(face.mIndices[j]);
                }
            }

            return Render::Object(vertices, indices);
        }
        ModelRenderer::ModelRenderer(std::string path) 
            : scene(LoadModel(Engine::Instance->resources.GetFile(path))), RenderComponent(Render::Object({}), "ModelRenderer")
        {
            object = CreateObject(scene);
        }

        ModelRenderer::ModelRenderer(tinyxml2::XMLElement* element) 
            : scene(LoadModel(Engine::Instance->resources.GetFile(element->Attribute("file")))), RenderComponent(element)
        {
            object = CreateObject(scene);
        }
        void ModelRenderer::Export(tinyxml2::XMLElement* element) {
            RenderComponent::Export(element);
            element->SetAttribute("file", path.c_str());
		}
		FactoryBase* modelfactory = GameObject::AddComponentFactory<ModelRenderer>(std::string("ModelRenderer"));
    }
}
#endif