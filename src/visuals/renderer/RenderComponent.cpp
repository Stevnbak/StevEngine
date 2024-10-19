#include "utilities/Vector3.hpp"
#ifdef StevEngine_RENDERER_GL
#include "RenderComponent.hpp"
#include "main/GameObject.hpp"
#include "RenderSystem.hpp"
#include "utilities/Color.hpp"
#include "visuals/Texture.hpp"
#include "visuals/shaders/Shader.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/quaternion.hpp"
#include <yaml-cpp/yaml.h>

using StevEngine::Utilities::Color;

namespace StevEngine::Renderer {
	//Constructor
	RenderComponent::RenderComponent(std::vector<Utilities::Vertex> vertices, std::string type)
		: Component(type), object(Object(vertices)) {}
	RenderComponent::RenderComponent(std::vector<Utilities::Vertex> vertices, Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale, std::string type)
		: Component(type), position(position), rotation(rotation), scale(scale), object(Object(vertices)) {}
	RenderComponent::RenderComponent(Object object, std::string type)
		: Component(type), object(object) {}
	RenderComponent::RenderComponent(Object object, Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale, std::string type)
		: Component(type), position(position), rotation(rotation), scale(scale), object(object) {}
	RenderComponent::RenderComponent(Object object, YAML::Node node)
		: Component(node), object(object), position(node["position"].as<Utilities::Vector3>()), rotation(node["rotation"].as<Utilities::Quaternion>()), scale(node["scale"].as<Utilities::Vector3>())
	{
		if(node["shaders"].IsSequence()) {
			for(int i = 0; i < node["shaders"].size(); i++)
				AddShader(ShaderProgram(node["shaders"][i]));
		}
		if(node["color"]) SetColor(node["color"].as<Color>());
		if(node["texture"] && node["texture"].as<std::string>() != "") SetTexture(Visuals::Texture(Resources::resourceManager.GetFile(node["texture"].as<std::string>())));
		if(node["normalMap"] && node["normalMap"].as<std::string>() != "") SetTexture(Visuals::Texture(Resources::resourceManager.GetFile(node["normalMap"].as<std::string>())));
	}
	//Destructor
	RenderComponent::~RenderComponent() {
		object.FreeTexture();
	}
	//Main draw function
	void RenderComponent::Draw(const Utilities::Matrix4& transform) {
		//New transform
		Utilities::Matrix4 trnsfm = Utilities::Matrix4::FromTranslationRotationScale(position, rotation, scale) * transform;
		//Draw
		Renderer::render.DrawObject(object, trnsfm);
	}
	void RenderComponent::SetColor(const Color& color) {
		this->color = color;
		object.color = color;
	}
	void RenderComponent::SetTexture(const Visuals::Texture& texture) {
		object.SetTexture(texture);
		texturePath = texture.GetPath();
	}
	void RenderComponent::SetNormalMap(const Visuals::Texture& normalMap) {
		object.SetNormalMap(normalMap);
		normalMapPath = normalMap.GetPath();
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
		node["color"] = color;
		node["texture"] = texturePath;
		node["normalMap"] = normalMapPath;
		for(auto&[type, program] : shaders)
			node["shaders"].push_back(program.Export());
		return node;
	}
}
#endif
