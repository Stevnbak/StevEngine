#ifdef StevEngine_RENDERER_GL
#include "Model.hpp"
#include "main/Engine.hpp"
#include "main/Log.hpp"
#include "main/ResourceManager.hpp"
#include "scenes/Component.hpp"
#include "visuals/render/Object.hpp"
#include "visuals/render/Component.hpp"
#include "visuals/Texture.hpp"
#include "utilities/Model.hpp"
#include "utilities/Color.hpp"
#include <vector>

using StevEngine::Utilities::Color;

namespace StevEngine {
	namespace Visuals {
		Render::Object CreateRenderObject(const Utilities::Model& model, Color color, Visuals::Texture surface) {
			std::vector<Utilities::Vertex> vertices;
			std::vector<unsigned int> indices;
			for(Utilities::Mesh mesh : model.GetMeshes()) {
				vertices.insert(vertices.end(), mesh.vertices.begin(), mesh.vertices.end());
				indices.insert(indices.end(), mesh.indices.begin(), mesh.indices.end());
			}
			return Render::Object(vertices, indices, color, surface);
		}
		ModelRenderer::ModelRenderer(const Utilities::Model& model)
			: model(model), RenderComponent(CreateRenderObject(model, Color(255, 255, 255, 255), Visuals::Texture::empty), "ModelRenderer") {}
		ModelRenderer::ModelRenderer(YAML::Node node)
			: model(Utilities::Model(Resources::resourceManager.GetFile(node["model"].as<std::string>()))), RenderComponent(CreateRenderObject(Utilities::Model(Resources::resourceManager.GetFile(node["model"].as<std::string>())), Utilities::Color(), Visuals::Texture::empty), node) {}
		YAML::Node ModelRenderer::Export(YAML::Node node) const {
			YAML::Node n = RenderComponent::Export(node);
			n["model"] = model.path;
			return n;
		}
	}
}
#endif
