#include "scenes/Component.hpp"
#include "utilities/Color.hpp"
#include "visuals/render/Component.hpp"
#ifdef StevEngine_RENDERER_GL
#include "Model.hpp"
#include "main/Engine.hpp"
#include "main/Log.hpp"
#include "main/ResourceManager.hpp"
#include "visuals/render/Object.hpp"
#include "utilities/Model.hpp"
#include "visuals/Texture.hpp"

#include <vector>

using StevEngine::Utilities::Color;

namespace StevEngine {
	namespace Visuals {
		Render::Object CreateRenderObject(Utilities::Model model, Color color, Visuals::Texture surface) {
			std::vector<Utilities::Vertex> vertices;
			std::vector<unsigned int> indices;
			for(Utilities::Mesh mesh : model.GetMeshes()) {
				vertices.insert(vertices.end(), mesh.vertices.begin(), mesh.vertices.end());
				indices.insert(indices.end(), mesh.indices.begin(), mesh.indices.end());
			}
			return Render::Object(vertices, indices, color, surface);
		}
		ModelRenderer::ModelRenderer(Utilities::Model model)
			: model(model), RenderComponent(CreateRenderObject(model, Color(255, 255, 255, 255), Visuals::Texture::empty), "ModelRenderer") {}
		ModelRenderer::ModelRenderer(YAML::Node node)
			: model(Utilities::Model(Engine::Instance->resources.GetFile(node["model"].as<std::string>()))), RenderComponent({}, node["position"].as<Utilities::Vector3>(), node["rotation"].as<Utilities::Quaternion>(), node["scale"].as<Utilities::Vector3>(), "ModelRenderer")
		{
			object = CreateRenderObject(model, Color(255, 255, 255, 255), Visuals::Texture::empty);
			if(node["color"]) SetColor(node["color"].as<Utilities::Color>());
			if(node["texture"] && node["texture"].as<std::string>() != "") SetTexture(Visuals::Texture(Engine::Instance->resources.GetFile(node["texture"].as<std::string>())));
		}
		YAML::Node ModelRenderer::Export(YAML::Node node) const {
			YAML::Node n = RenderComponent::Export(node);
			n["model"] = model.path;
			n["color"] = color;
			n["texture"] = texturePath;
			return n;
		}
	}
}
#endif
