#ifdef StevEngine_RENDERER_GL
#include "RenderComponent.hpp"
#include "RenderSystem.hpp"
#include "visuals/shaders/Shader.hpp"
#include "utilities/Vector3.hpp"

#include <yaml-cpp/yaml.h>

namespace StevEngine::Renderer {
	//Constructor
	RenderComponent::RenderComponent(const Object& object, std::string type)
		: Component(type), object(object) {}
	RenderComponent::RenderComponent(const Object& object, Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale, std::string type)
		: Component(type), position(position), rotation(rotation), scale(scale), object(object) {}
	RenderComponent::RenderComponent(const Object& object, YAML::Node node)
		: Component(node), object(object), position(node["position"].as<Utilities::Vector3>()), rotation(node["rotation"].as<Utilities::Quaternion>()), scale(node["scale"].as<Utilities::Vector3>())
	{
		if(node["shaders"].IsSequence()) {
			for(int i = 0; i < node["shaders"].size(); i++)
				AddShader(ShaderProgram(node["shaders"][i]));
		}
	}
	//Destructor
	RenderComponent::~RenderComponent() {
		object.material.FreeAlbedo();
		object.material.FreeNormal();
	}
	//Main draw function
	void RenderComponent::Draw(const Utilities::Matrix4& transform) {
		//New transform
		Utilities::Matrix4 trnsfm = Utilities::Matrix4::FromTranslationRotationScale(position, rotation, scale) * transform;
		//Draw
		Renderer::render.DrawObject(object, trnsfm);
	}
	void RenderComponent::AddShader(ShaderProgram program) {
		object.AddShader(program);
		if(shaders.contains(program.GetType())) shaders[program.GetType()] = program;
		else shaders.insert({program.GetType(), program});
	}
	void RenderComponent::RemoveShader(ShaderType type) {
		object.RemoveShader(type);
		if(shaders.contains(type)) shaders.erase(type);
	}
	YAML::Node RenderComponent::Export(YAML::Node node) const {
		node["position"] = position;
		node["rotation"] = rotation;
		node["scale"] = scale;
		node["material"] = object.material.Export();
		for(auto&[type, program] : shaders)
			node["shaders"].push_back(program.Export());
		return node;
	}
}
#endif
