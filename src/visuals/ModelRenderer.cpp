#ifdef StevEngine_RENDERER_GL
#include "ModelRenderer.hpp"
#include "main/ResourceManager.hpp"

#include "utilities/Stream.hpp"
#include "visuals/renderer/RenderSystem.hpp"
#include "visuals/renderer/Object.hpp"
#include "visuals/Material.hpp"
#include "utilities/Model.hpp"
#include <vector>

using namespace StevEngine::Renderer;

namespace StevEngine::Visuals {
	std::vector<Renderer::Object> CreateRenderObjects(const Utilities::Model& model, const Material& material) {
		std::vector<Renderer::Object> objects;
		const auto& meshes = model.GetMeshes();
		objects.reserve(meshes.size());
		for(const auto& mesh : meshes) {
			std::vector<Utilities::Vertex> vertices;
			vertices.reserve(mesh.vertices.size());
			std::vector<uint32_t> indices;
			indices.reserve(mesh.indices.size());

			vertices.insert(vertices.end(), mesh.vertices.begin(), mesh.vertices.end());
			indices.insert(indices.end(), mesh.indices.begin(), mesh.indices.end());

			objects.push_back(Renderer::Object(vertices, indices, model.hasMaterials ? mesh.material : material));
		}
		return objects;
	}
	ModelRenderer::ModelRenderer(const Utilities::Model& model, const Material& material, Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale)
	  : model(model), objects(CreateRenderObjects(model, material)) {}
	ModelRenderer::ModelRenderer(Stream& stream)
	  : model(Utilities::Model(Resources::resourceManager.GetFile(stream.Read<std::string>()))),
		objects(CreateRenderObjects(model,
		Material(stream.Read<Material>())))
	{
		uint shaderCount = stream.Read<uint>();
		for(int i = 0; i < shaderCount; i++)
			AddShader(ShaderProgram(stream));
	}

	Stream ModelRenderer::Export(StreamType type) const {
		Stream stream(type);
		stream << model.path << position << rotation << scale;
		stream << (uint)shaders.size();
		for(auto&[_, program] : shaders)
			stream << program.Export(type);
		return stream;
	}

	//Destructor
	ModelRenderer::~ModelRenderer() {
		for(auto& object : objects) {
			object.material.FreeAlbedo();
			object.material.FreeNormal();
		}
	}
	//Main draw function
	void ModelRenderer::Draw(const Utilities::Matrix4& transform) {
		//New transform
		Utilities::Matrix4 trnsfm = Utilities::Matrix4::FromTranslationRotationScale(position, rotation, scale) * transform;
		//Draw objects
		for(const auto& object : objects) {
			Renderer::render.DrawObject(object, trnsfm);
		}
	}
	void ModelRenderer::AddShader(ShaderProgram program) {
		for(auto& object : objects) object.AddShader(program);
		if(shaders.contains(program.GetType())) shaders[program.GetType()] = program;
		else shaders.insert({program.GetType(), program});
	}
	void ModelRenderer::RemoveShader(ShaderType type) {
		for(auto& object : objects) object.RemoveShader(type);
		if(shaders.contains(type)) shaders.erase(type);
	}
}
#endif
