#include "Primitive.hpp"
#include <main/Log.hpp>
#include <utilities/Vector3.hpp>
#include <utilities/Quaternion.hpp>
#include <scenes/GameObject.hpp>

#include <SDL2/SDL.h>
#include <glad/glad.h> 
#include <math.h>

namespace StevEngine {
	namespace Visuals {
		std::vector<float> CubeVertices() {
			return {
					0.5, -0.5, -0.5,
					0.5, -0.5, 0.5,
					0.5, 0.5, 0.5,
					0.5, 0.5, -0.5,
					-0.5, -0.5, -0.5,
					-0.5, -0.5, 0.5,
					-0.5, 0.5, 0.5,
					-0.5, 0.5, -0.5
				};
		}
		std::vector<unsigned int> CubeIndices() {
			return {
					//Right
					0, 1, 3,
					1, 2, 3,
					//Left
					4, 5, 7,
					5, 6, 7,
					//Top
					3, 2, 6,
					3, 7, 6,
					//Bottom
					0, 1, 5,
					0, 4, 5,
					//Back
					1, 2, 5,
					2, 6, 5,
					//Front
					0, 3, 7,
					0, 4, 7
				};
		}
		CubePrimitive::CubePrimitive(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale) 
			: Render::RenderComponent(
				//Vertices
				CubeVertices(),
				//Indices
				CubeIndices(), 
				position, rotation, scale, "CubePrimitive") {}
		CubePrimitive::CubePrimitive(tinyxml2::XMLElement* element) : RenderComponent(element) { 
			this->vertices = CubeVertices();
			this->indices = CubeIndices();
		}
		FactoryBase* cubefactory = GameObject::AddComponentFactory<CubePrimitive>(std::string("CubePrimitive"));

		SpherePrimitive::SpherePrimitive(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale) 
			: Render::RenderComponent({}, {}, position, rotation, scale, "SpherePrimitive") {}
		SpherePrimitive::SpherePrimitive(tinyxml2::XMLElement* element) : RenderComponent(element) {}
		FactoryBase* spherefactory = GameObject::AddComponentFactory<SpherePrimitive>(std::string("SpherePrimitive"));

		CylinderPrimitive::CylinderPrimitive(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale) 
			: Render::RenderComponent({}, {}, position, rotation, scale, "CylinderPrimitive") {}
		CylinderPrimitive::CylinderPrimitive(tinyxml2::XMLElement* element) : RenderComponent(element) {}
		FactoryBase* cylinderfactory = GameObject::AddComponentFactory<CylinderPrimitive>(std::string("CylinderPrimitive"));

		CapsulePrimitive::CapsulePrimitive(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale) 
			: Render::RenderComponent({}, {}, position, rotation, scale, "CapsulePrimitive") {}
		CapsulePrimitive::CapsulePrimitive(tinyxml2::XMLElement* element) : RenderComponent(element) {}
		FactoryBase* capsulefactory = GameObject::AddComponentFactory<CapsulePrimitive>(std::string("CapsulePrimitive"));
	}
}