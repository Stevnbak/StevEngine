#ifdef StevEngine_RENDERER_GL
#include "ModelRenderer.hpp"
#include "main/Engine.hpp"
#include "main/Log.hpp"
#include "main/ResourceManager.hpp"
#include "main/Component.hpp"
#include "visuals/renderer/Object.hpp"
#include "visuals/renderer/RenderComponent.hpp"
#include "visuals/Material.hpp"
#include "utilities/Model.hpp"
#include "utilities/Color.hpp"
#include <vector>

namespace StevEngine::Visuals {
	Renderer::Object CreateRenderObject(const Utilities::Model& model, const Material& material) {
		std::vector<Utilities::Vertex> vertices;
		std::vector<uint32_t> indices;
		for(Utilities::Mesh mesh : model.GetMeshes()) {
			vertices.insert(vertices.end(), mesh.vertices.begin(), mesh.vertices.end());
			indices.insert(indices.end(), mesh.indices.begin(), mesh.indices.end());
		}
		return Renderer::Object(vertices, indices, material);
	}
	ModelRenderer::ModelRenderer(const Utilities::Model& model, const Material& material)
		: model(model), RenderComponent(CreateRenderObject(model, material), "ModelRenderer") {}
	ModelRenderer::ModelRenderer(YAML::Node node)
		: model(Utilities::Model(Resources::resourceManager.GetFile(node["model"].as<std::string>()))), RenderComponent(CreateRenderObject(Utilities::Model(Resources::resourceManager.GetFile(node["model"].as<std::string>())), Material(node["material"])), node) {}
	YAML::Node ModelRenderer::Export(YAML::Node node) const {
		YAML::Node n = RenderComponent::Export(node);
		n["model"] = model.path;
		return n;
	}
}
#endif
