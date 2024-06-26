#include "Component.hpp"
#include "scenes/GameObject.hpp"
#include "main/Engine.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace StevEngine {
    namespace Render {
        //Constructor
        RenderComponent::RenderComponent(std::vector<float> vertices, std::vector<unsigned int> indices, std::string type) 
            : Component(type), vertices(vertices), indices(indices) {}
		RenderComponent::RenderComponent(std::vector<float> vertices, std::vector<unsigned int> indices, Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale, std::string type)
            : Component(type), vertices(vertices), indices(indices), position(position), rotation(rotation), scale(scale) {}

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
			Engine::Instance->render.DrawObject(Render::Object(vertices, indices, transform, color));
		}
		//Export and import
		void RenderComponent::Export(tinyxml2::XMLElement* element) {
			element->SetAttribute("position", ((std::string)position).c_str());
			element->SetAttribute("rotation", ((std::string)rotation).c_str());
			element->SetAttribute("scale", ((std::string)scale).c_str());
			element->SetAttribute("color", std::format("[{},{},{},{}]", color.r, color.g, color.b, color.a).c_str());
		}
		RenderComponent::RenderComponent(tinyxml2::XMLElement* node) : Component(node->Attribute("type")) {
			position = Utilities::Vector3(node->Attribute("position"));
			rotation = Utilities::Quaternion(node->Attribute("rotation"));
			scale = Utilities::Vector3(node->Attribute("scale"));
			std::string str = node->Attribute("color");
			if(!str.starts_with("[") || !str.ends_with("]") || !str.contains(",")) {
				Log::Error("RenderComponent color string not valid.", true);
			} else {
				//Create stream
				std::istringstream ss(str.substr(1, str.length() - 2));
				std::string s;
				//Get values
				std::getline(ss, s, ',');
				color.r = stod(s);
				std::getline(ss, s, ',');
				color.g = stod(s);
				std::getline(ss, s, ',');
				color.b = stod(s);
				std::getline(ss, s, ',');
				color.a = stod(s);
			}
		}
    }
}