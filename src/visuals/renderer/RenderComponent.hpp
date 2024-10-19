#pragma once
#ifdef StevEngine_RENDERER_GL
#include "Object.hpp"

#include <SDL.h>
#include <glad/gl.h>
#include <yaml-cpp/yaml.h>

#include "utilities/Vector3.hpp"
#include "utilities/Vertex.hpp"
#include "utilities/Quaternion.hpp"
#include "visuals/Texture.hpp"
#include "utilities/Color.hpp"
#include "main/Component.hpp"
#include "visuals/shaders/Shader.hpp"

namespace StevEngine::Renderer {
	class RenderComponent : public Component {
		friend class StevEngine::GameObject;

		public:
			RenderComponent(Object object, std::string type = "RenderComponent");
			RenderComponent(Object object, Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale, std::string type);
			RenderComponent(std::vector<Utilities::Vertex> vertices, std::string type = "RenderComponent");
			RenderComponent(std::vector<Utilities::Vertex> vertices, Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1), std::string type = "RenderComponent");
			RenderComponent(Object object, YAML::Node node);
			//Basic properties
			Utilities::Vector3 position = Utilities::Vector3();
			Utilities::Quaternion rotation = Utilities::Quaternion();
			Utilities::Vector3 scale = Utilities::Vector3(1, 1, 1);
			Object& GetObject() { return object; };
			void SetColor(const Utilities::Color& color);
			void SetTexture(const Visuals::Texture& texture);
			void SetNormalMap(const Visuals::Texture& normalMap);
			void AddShader(ShaderProgram program);
			void RemoveShader(ShaderType type);
			~RenderComponent();
			virtual YAML::Node Export(YAML::Node node) const;
		protected:
			Object object;
			Utilities::Color color = Utilities::Color(255, 255, 255, 255);
			std::string texturePath, normalMapPath;
			std::map<ShaderType, ShaderProgram> shaders;
		//Main functions
		private:
			void Draw(const glm::mat4x4& transform);
	};
}
#endif
