#ifdef StevEngine_RENDERER_GL
#include "Model.hpp"
#include "main/Engine.hpp"
#include "main/Log.hpp"
#include "main/ResourceManager.hpp"
#include "visuals/render/Object.hpp"
#include "utilities/Model.hpp"
#include "utilities/Texture.hpp"

#include <vector>

namespace StevEngine {
    namespace Visuals {
        Render::Object CreateRenderObject(Utilities::Model model, SDL_Color color, Utilities::Texture surface) {
            std::vector<Utilities::Vertex> vertices;
            std::vector<unsigned int> indices;
            for(Utilities::Mesh mesh : model.GetMeshes()) {
                vertices.insert(vertices.end(), mesh.vertices.begin(), mesh.vertices.end());
                indices.insert(indices.end(), mesh.indices.begin(), mesh.indices.end());
            }
            return Render::Object(vertices, indices, color, surface);
        }
        ModelRenderer::ModelRenderer(Utilities::Model model)
            : model(model), RenderComponent(CreateRenderObject(model, (SDL_Color){1, 1, 1, 1}, nullptr), "ModelRenderer") {}

        ModelRenderer::ModelRenderer(tinyxml2::XMLElement* element)
            : model(Utilities::Model(Engine::Instance->resources.GetFile(element->Attribute("file")))), RenderComponent(element)
        {
            object = CreateRenderObject(model, color, nullptr);
        }
        void ModelRenderer::Export(tinyxml2::XMLElement* element) {
            RenderComponent::Export(element);
            element->SetAttribute("file", model.path.c_str());
		}
		FactoryBase* modelfactory = GameObject::AddComponentFactory<ModelRenderer>(std::string("ModelRenderer"));
    }
}
#endif
