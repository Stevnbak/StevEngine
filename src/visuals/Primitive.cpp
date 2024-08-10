#ifdef StevEngine_RENDERER_GL
#include "Primitive.hpp"
#include "visuals/render/Object.hpp"
#include <main/Log.hpp>
#include <utilities/Vector3.hpp>
#include <utilities/Quaternion.hpp>
#include <scenes/GameObject.hpp>

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <math.h>

using namespace StevEngine::Render;
using StevEngine::Utilities::Vertex;

namespace StevEngine {
	namespace Visuals {
		std::vector<Vertex> CubeVertices() {
			return {
				//Right
				Vertex(0.5, -0.5, -0.5),
				Vertex(0.5, -0.5, 0.5),
				Vertex(0.5, 0.5, -0.5),

				Vertex(0.5, -0.5, 0.5),
				Vertex(0.5, 0.5, 0.5),
				Vertex(0.5, 0.5, -0.5),

				//Left
				Vertex(-0.5, -0.5, -0.5),
				Vertex(-0.5, -0.5, 0.5),
				Vertex(-0.5, 0.5, -0.5),

				Vertex(-0.5, -0.5, 0.5),
				Vertex(-0.5, 0.5, 0.5),
				Vertex(-0.5, 0.5, -0.5),

				//Top
				Vertex(0.5, 0.5, -0.5),
				Vertex(0.5, 0.5, 0.5),
				Vertex(-0.5, 0.5, 0.5),

				Vertex(0.5, 0.5, -0.5),
				Vertex(-0.5, 0.5, -0.5),
				Vertex(-0.5, 0.5, 0.5),

				//Bottom
				Vertex(0.5, -0.5, -0.5),
				Vertex(0.5, -0.5, 0.5),
				Vertex(-0.5, -0.5, 0.5),

				Vertex(0.5, -0.5, -0.5),
				Vertex(-0.5, -0.5, -0.5),
				Vertex(-0.5, -0.5, 0.5),

				//Back
				Vertex(0.5, -0.5, 0.5),
				Vertex(0.5, 0.5, 0.5),
				Vertex(-0.5, -0.5, 0.5),

				Vertex(0.5, 0.5, 0.5),
				Vertex(-0.5, 0.5, 0.5),
				Vertex(-0.5, -0.5, 0.5),

				//Front
				Vertex(0.5, -0.5, -0.5),
				Vertex(0.5, 0.5, -0.5),
				Vertex(-0.5, 0.5, -0.5),

				Vertex(0.5, -0.5, -0.5),
				Vertex(-0.5, -0.5, -0.5),
				Vertex(-0.5, 0.5, -0.5)
				};
		}
		CubePrimitive::CubePrimitive(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale)
			: RenderComponent(CubeVertices(), position, rotation, scale, "CubePrimitive") {}
		CubePrimitive::CubePrimitive(tinyxml2::XMLElement* element) : RenderComponent(element) {
			object = Object(CubeVertices(), color);
		}
		FactoryBase* cubefactory = GameObject::AddComponentFactory<CubePrimitive>(std::string("CubePrimitive"));

		SpherePrimitive::SpherePrimitive(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale)
			: RenderComponent({}, position, rotation, scale, "SpherePrimitive") {}
		SpherePrimitive::SpherePrimitive(tinyxml2::XMLElement* element) : RenderComponent(element) {}
		FactoryBase* spherefactory = GameObject::AddComponentFactory<SpherePrimitive>(std::string("SpherePrimitive"));

		CylinderPrimitive::CylinderPrimitive(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale)
			: RenderComponent({}, position, rotation, scale, "CylinderPrimitive") {}
		CylinderPrimitive::CylinderPrimitive(tinyxml2::XMLElement* element) : RenderComponent(element) {}
		FactoryBase* cylinderfactory = GameObject::AddComponentFactory<CylinderPrimitive>(std::string("CylinderPrimitive"));

		CapsulePrimitive::CapsulePrimitive(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale)
			: RenderComponent({}, position, rotation, scale, "CapsulePrimitive") {}
		CapsulePrimitive::CapsulePrimitive(tinyxml2::XMLElement* element) : RenderComponent(element) {}
		FactoryBase* capsulefactory = GameObject::AddComponentFactory<CapsulePrimitive>(std::string("CapsulePrimitive"));
	}
}
#endif
