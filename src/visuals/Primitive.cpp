#ifdef StevEngine_RENDERER_GL
#include "Primitive.hpp"
#include "visuals/render/Object.hpp"
#include <main/Log.hpp>
#include <utilities/Vector3.hpp>
#include "utilities/Vertex.hpp"
#include <utilities/Quaternion.hpp>
#include <scenes/GameObject.hpp>

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <math.h>
#include <array>
#include <vector>

using namespace StevEngine::Render;
using StevEngine::Utilities::Vertex;
using StevEngine::Utilities::Vector3;

namespace StevEngine {
	namespace Visuals {
		std::vector<Vertex> CubeVertices() {
			std::array<Vertex, 6*6> vertices;
			for(int side = 0; side < 6; side++) {
				std::array<Vector3, 6> sideVertices;
				int a = (side % 2 ? 1 : -1);
				for(int tri = -1; tri < 2; tri += 2) {
                    for(int ver = 0; ver < 3; ver++) {
                        int b = (ver == 0 ? 1 : (ver == 1 ? tri : (-1)));
                        int c = (ver == 0 ? 1 : (ver == 1 ? -1 : (-tri)));
						int offset = tri < 0 ? 0 : 3;
                        if(side < 2) sideVertices[offset + ver] = Vector3(a, b, c);
                        else if(side < 4) sideVertices[offset + ver] = Vector3(b, a, c);
                        else sideVertices[offset + ver] = Vector3(c, b, a);
                    }
				}
				Vector3 normal = Vector3::Cross(sideVertices[1] - sideVertices[0], sideVertices[2] - sideVertices[0]).Normalized();
				for(int i = 0; i < 6; i++) {
				    vertices[side * 6 + i] = Vertex(sideVertices[i] * 0.5, normal);
				}
			}
			return std::vector<Vertex>(vertices.begin(), vertices.end());
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
