#pragma once
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
				RenderComponent(std::vector<Utilities::Vertex> vertices, std::string type = "RenderComponent");
				RenderComponent(std::vector<Utilities::Vertex> vertices, Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1), std::string type = "RenderComponent");
				RenderComponent(tinyxml2::XMLElement* element);
				//Basic properties
				Utilities::Vector3 position = Utilities::Vector3();
				Utilities::Quaternion rotation = Utilities::Quaternion();
				Utilities::Vector3 scale = Utilities::Vector3(1, 1, 1);
				void SetColor(Utilities::Color color);
				void SetTexture(Visuals::Texture texture) { object.SetTexture(texture); }
				void AddShader(Render::ShaderProgram program) { object.AddShader(program); }
                void RemoveShader(Render::ShaderType type) { object.RemoveShader(type); }
				~RenderComponent();
			protected:
				Object object;
				Utilities::Color color = Utilities::Color(255, 255, 255, 255);
			//Main functions
			private:
				void Update(double deltaTime) {};
				void Draw(glm::mat4x4 transform);
				void Deactivate() {};
				void Start() {};
			protected:
				virtual void Export(tinyxml2::XMLElement* element);
		};
    }
}
#endif
