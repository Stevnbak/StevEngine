#pragma once
#include "yaml-cpp/node/node.h"
#ifdef StevEngine_RENDERER_GL
#include "Object.hpp"

#include <SDL.h>
#include <glad/gl.h>

#include "utilities/Vector3.hpp"
#include "utilities/Vertex.hpp"
#include "utilities/Quaternion.hpp"
#include "visuals/Texture.hpp"
#include "utilities/Color.hpp"
#include "scenes/Component.hpp"

namespace StevEngine {
	namespace Render {
		class RenderComponent : public Component {
			friend class StevEngine::GameObject;

			public:
				RenderComponent(Object object, std::string type = "RenderComponent");
				RenderComponent(Object object, Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale, std::string type);
				RenderComponent(std::vector<Utilities::Vertex> vertices, std::string type = "RenderComponent");
				RenderComponent(std::vector<Utilities::Vertex> vertices, Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1), std::string type = "RenderComponent");
				//Basic properties
				Utilities::Vector3 position = Utilities::Vector3();
				Utilities::Quaternion rotation = Utilities::Quaternion();
				Utilities::Vector3 scale = Utilities::Vector3(1, 1, 1);
				void SetColor(Utilities::Color color);
				void SetTexture(Visuals::Texture texture);
				void AddShader(Render::ShaderProgram program) { object.AddShader(program); }
				void RemoveShader(Render::ShaderType type) { object.RemoveShader(type); }
				~RenderComponent();
				virtual YAML::Node Export(YAML::Node node) const;
			protected:
				Object object;
				Utilities::Color color = Utilities::Color(255, 255, 255, 255);
				std::string texturePath;
			//Main functions
			private:
				void Update(double deltaTime) {};
				void Draw(glm::mat4x4 transform);
				void Deactivate() {};
				void Start() {};
		};
	}
}
#endif
