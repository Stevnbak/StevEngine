#pragma once
#ifdef StevEngine_RENDERER_GL
#include "Object.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h> 
#include <utilities/Vector3.hpp>
#include <utilities/Quaternion.hpp>
#include <scenes/Component.hpp>

namespace StevEngine {
    namespace Render {
        class RenderComponent : public Component {
			friend class StevEngine::GameObject;

			public:
				RenderComponent(std::vector<Vertex> vertices, std::string type = "RenderComponent");
				RenderComponent(std::vector<Vertex> vertices, Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1), std::string type = "RenderComponent");
				RenderComponent(tinyxml2::XMLElement* element);
				//Basic properties
				Utilities::Vector3 position = Utilities::Vector3();
				Utilities::Quaternion rotation = Utilities::Quaternion();
				Utilities::Vector3 scale = Utilities::Vector3(1, 1, 1);
				void SetColor(SDL_Color color);
			protected:
				std::vector<Vertex> vertices;
			private:
				Object object;
				SDL_Color color = SDL_Color(1,1,1,1);
				void RefreshObject();
			//Main functions
			private:
				void Update(double deltaTime) {};
				void Draw(glm::mat4x4 transform);
				void Deactivate() {};
				void Start() {};
				void Export(tinyxml2::XMLElement* element);
		};
    }
}
#endif