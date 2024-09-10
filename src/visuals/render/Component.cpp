#include "utilities/Color.hpp"
#include "visuals/Texture.hpp"
#include "visuals/shaders/Shader.hpp"
#include "yaml-cpp/node/node.h"
#ifdef StevEngine_RENDERER_GL
#include "Component.hpp"
#include "scenes/GameObject.hpp"
#include "main/Engine.hpp"

#include <glm/gtc/matrix_transform.hpp>

using StevEngine::Utilities::Color;

namespace StevEngine {
	namespace Render {
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
			if(node["texture"] && node["texture"].as<std::string>() != "") SetTexture(Visuals::Texture(Engine::Instance->resources.GetFile(node["texture"].as<std::string>())));
		}
		//Destructor
		RenderComponent::~RenderComponent() {
			object.FreeTexture();
		}
		//Main draw function
		void RenderComponent::Draw(glm::mat4x4 transform) {
		 	//Scale
			transform = glm::scale(transform, glm::vec3(scale.X, scale.Y, scale.Z));
			//Move
			transform = glm::translate(transform, glm::vec3(position.X, position.Y, position.Z));
			//Rotate
			std::tuple<double, Utilities::Vector3> angleAxis = rotation.GetAngleAxis();
			Utilities::Vector3 v = std::get<1>(angleAxis);
			transform = glm::rotate(transform, (float)std::get<0>(angleAxis), glm::vec3(v.X, v.Y, v.Z));
			//Draw
			Engine::Instance->render.DrawObject(object, transform);
		}
		void RenderComponent::SetColor(Color color) {
			this->color = color;
			object.color = color;
		}
		void RenderComponent::SetTexture(Visuals::Texture texture) {
			object.SetTexture(texture);
			texturePath = texture.path;
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
			for(std::pair<ShaderType, ShaderProgram> p : shaders)
				node["shaders"].push_back(p.second.Export());
			return node;
		}
	}
}
#endif
