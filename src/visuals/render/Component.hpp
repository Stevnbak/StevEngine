#pragma once
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
				RenderComponent(std::vector<float> vertices, std::vector<unsigned int> indices, std::string type = "RenderComponent");
				RenderComponent(std::vector<float> vertices, std::vector<unsigned int> indices, Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1), std::string type = "RenderComponent");
				RenderComponent(tinyxml2::XMLElement* element);
				//Basic properties
				Utilities::Vector3 position = Utilities::Vector3();
				Utilities::Quaternion rotation = Utilities::Quaternion();
				Utilities::Vector3 scale = Utilities::Vector3(1, 1, 1);
				SDL_Color color = SDL_Color(1,1,1,1);
			protected:
				std::vector<float> vertices;
                std::vector<unsigned int> indices;
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