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
using namespace StevEngine::Utilities;

namespace StevEngine {
	namespace Visuals {
		double r = 0.5;
	    //Cube
		const std::vector<Vertex> CubeVertices() {
			std::array<Vertex, 6*6> vertices;
			for(int side = 0; side < 6; side++) {
			    float sideEffect = side % 2;
			    //Texture coordinates
				Vector2 sideUV;
				if(side < 1)      sideUV = Vector2(1, 1);
				else if(side < 2) sideUV = Vector2(3, 1);
				else if(side < 3) sideUV = Vector2(1, 2);
				else if(side < 4) sideUV = Vector2(1, 0);
				else if(side < 5) sideUV = Vector2(0, 1);
				else              sideUV = Vector2(2, 1);
				//Coordinates
				int a = (sideEffect ? 1 : -1);
				std::array<Vector3, 6> sideVertices;
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
				//Normal
				Vector3 normal = Vector3::Cross(sideVertices[2] - sideVertices[0], sideVertices[1] - sideVertices[0]).Normalized();
				//Combine to vertices
				for(int i = 0; i < 6; i++) {
                    float u = side < 2 ? sideVertices[i].Y : side < 4 ? sideVertices[i].X : sideVertices[i].X;
					float v = side < 2 ? sideVertices[i].Z : side < 4 ? sideVertices[i].Z : sideVertices[i].Y;
				    vertices[side * 6 + i] = Vertex(
						sideVertices[i] * r,
						normal,
						Vector2(std::abs(sideUV.X + ((u + 1) / 2)) * 0.25, std::abs((sideEffect) - (sideUV.Y + ((v + 1) / 2)) * 0.333333333333))
					);
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

		//Sphere
		const std::vector<Vertex> SphereVertices(int detail = 30, double height = r) {
            float anglePerStep = (2 * M_PI) / detail; //In Radians
			//Calculate circle layers
			std::vector<std::vector<Utilities::Vector3>> layers;
			for (int stack = -detail / 2; stack < detail / 2; stack++) {
				float stackAngle = anglePerStep * stack;
				float currentHeight = std::sin(stackAngle) * height;
				float cosStackAngle = std::cos(stackAngle);
				std::vector<Utilities::Vector3> circle;
				for (int section = 0; section <= detail; section++)
				{
					float sectionAngle = (anglePerStep)*section;
					circle.push_back(Utilities::Vector3(r * std::cos(sectionAngle) * cosStackAngle, currentHeight, r * std::sin(sectionAngle) * cosStackAngle));
				}
				layers.push_back(circle);
			}
			//Create triangles
			std::vector<Vertex> vertices;
			for (int i = 0; i < layers.size() - 1; i++) {
				std::vector<Vector3> circle1 = layers[i];
				std::vector<Vector3> circle2 = layers[i + 1];
				for (int j = 0; j < circle1.size() - 1; j++) {
					float currentAngle = (anglePerStep)*i;
					Vector3 a(circle1[j + 0].X, circle1[j + 0].Y, circle1[j + 0].Z);
					Vector3 b(circle1[j + 1].X, circle1[j + 1].Y, circle1[j + 1].Z);
					Vector3 c(circle2[j + 1].X, circle2[j + 1].Y, circle2[j + 1].Z);
					Vector3 d(circle2[j + 0].X, circle2[j + 0].Y, circle2[j + 0].Z);
					Vector3 normal = Vector3::Cross(c - a, b - a) * -1;

					vertices.push_back(Vertex(a, normal));
					vertices.push_back(Vertex(b, normal));
					vertices.push_back(Vertex(c, normal));

					vertices.push_back(Vertex(a, normal));
					vertices.push_back(Vertex(c, normal));
					vertices.push_back(Vertex(d, normal));
				}
			}
			return vertices;
		}
		SpherePrimitive::SpherePrimitive(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale)
			: RenderComponent(SphereVertices(), position, rotation, scale, "SpherePrimitive") {}
		SpherePrimitive::SpherePrimitive(tinyxml2::XMLElement* element) : RenderComponent(element) {
		    object = Object(SphereVertices(), color);
		}
		FactoryBase* spherefactory = GameObject::AddComponentFactory<SpherePrimitive>(std::string("SpherePrimitive"));

		//Cylinder
		const std::vector<Vertex> CylinderVertices(int detail = 50) {
		    //Create circle
            float anglePerStep = (2 * M_PI) / detail; //In Radians
            std::vector<Utilities::Vector2> circle;
           	for (int i = 0; i <= detail; i++)
           	{
          		float currentAngle = (anglePerStep) * i;
          		circle.push_back(Utilities::Vector2(r * std::cos(currentAngle), r * std::sin(currentAngle)));
           	}
			//Create triangles
			std::vector<Vertex> vertices;
			// Bottom
			Vector3 bottomCenter = Vector3(0, -r, 0);
			Vector3 bottomNormal = Vector3::Cross(Vector3(circle[2].X, -r, circle[2].Y) - bottomCenter, Vector3(circle[1].X, -r, circle[1].Y) - bottomCenter).Normalized();
			for (int i = 0; i < circle.size() - 1; i++) {
			    Utilities::Vector2 a = circle[i];
			    Utilities::Vector2 b = circle[i + 2];
				vertices.push_back(Vertex(Vector3(a.X, -r, a.Y), bottomNormal));
				vertices.push_back(Vertex(Vector3(b.X, -r, b.Y), bottomNormal));
				vertices.push_back(Vertex(bottomCenter, bottomNormal));
			}
			// Top
			Vector3 topCenter = Vector3(0, r, 0);
			Vector3 topNormal = Vector3::Cross(Vector3(circle[2].X, r, circle[2].Y) - topCenter, Vector3(circle[1].X, r, circle[1].Y) - topCenter).Normalized();
			for (int i = 0; i < circle.size() - 1; i++) {
			    Utilities::Vector2 a = circle[i];
			    Utilities::Vector2 b = circle[i + 2];
				vertices.push_back(Vertex(Vector3(a.X, r, a.Y), topNormal));
				vertices.push_back(Vertex(Vector3(b.X, r, b.Y), topNormal));
				vertices.push_back(Vertex(topCenter, topNormal));
			}
			// Sides
			for (int i = 0; i < circle.size() - 1; i++) {
			    Utilities::Vector2 a = circle[i];
			    Utilities::Vector2 b = circle[i + 2];

				Vector3 normal = Vector3::Cross(Vector3(b.X, r, b.Y) - Vector3(a.X, -r, a.Y), Vector3(b.X, -r, b.Y) - Vector3(a.X, -r, a.Y)).Normalized();

				vertices.push_back(Vertex(Vector3(a.X, -r, a.Y), normal));
				vertices.push_back(Vertex(Vector3(b.X, r, b.Y), normal));
				vertices.push_back(Vertex(Vector3(b.X, -r, b.Y), normal));

				vertices.push_back(Vertex(Vector3(a.X, r, a.Y), normal));
				vertices.push_back(Vertex(Vector3(b.X, r, b.Y), normal));
				vertices.push_back(Vertex(Vector3(a.X, -r, a.Y), normal));
			}
			//Return
			return vertices;
		}
		CylinderPrimitive::CylinderPrimitive(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale)
			: RenderComponent(CylinderVertices(), position, rotation, scale, "CylinderPrimitive") {}
		CylinderPrimitive::CylinderPrimitive(tinyxml2::XMLElement* element) : RenderComponent(element) {
		    object = Object(CylinderVertices(), color);
		}
		FactoryBase* cylinderfactory = GameObject::AddComponentFactory<CylinderPrimitive>(std::string("CylinderPrimitive"));

		//Capsule
		const std::vector<Vertex> CapsuleVertices(int detail = 30) {
			//Create triangles
			std::vector<Vertex> vertices;
			// Get sphere vertices
			std::vector<Vertex> sphere = SphereVertices(detail, r/2);
			// Bottom sphere
			for(int i = 0; i < sphere.size() / 2; i++) {
			    Vertex v = sphere[i];
				v.y = v.y - (r);
				vertices.push_back(v);
			}
			// Top sphere
			for(int i = sphere.size() / 2; i < sphere.size(); i++) {
			    Vertex v = sphere[i];
				v.y = v.y + (r);
				vertices.push_back(v);
			}
			// Sides
			float anglePerStep = (2 * M_PI) / detail; //In Radians
			float height = r;
            std::vector<Utilities::Vector2> circle;
           	for (int i = 0; i <= detail; i++)
           	{
          		float currentAngle = (anglePerStep) * i;
          		circle.push_back(Utilities::Vector2(r * std::cos(currentAngle), r * std::sin(currentAngle)));
           	}
			for (int i = 0; i < circle.size() - 1; i++) {
			    Utilities::Vector2 a = circle[i];
			    Utilities::Vector2 b = circle[i + 2];

				Vector3 normal = Vector3::Cross(Vector3(b.X, height, b.Y) - Vector3(a.X, -height, a.Y), Vector3(b.X, -height, b.Y) - Vector3(a.X, -height, a.Y)).Normalized();

				vertices.push_back(Vertex(Vector3(a.X, -height, a.Y), normal));
				vertices.push_back(Vertex(Vector3(b.X, height, b.Y), normal));
				vertices.push_back(Vertex(Vector3(b.X, -height, b.Y), normal));

				vertices.push_back(Vertex(Vector3(a.X, height, a.Y), normal));
				vertices.push_back(Vertex(Vector3(b.X, height, b.Y), normal));
				vertices.push_back(Vertex(Vector3(a.X, -height, a.Y), normal));
			}
			//Return
			return vertices;
		}
		CapsulePrimitive::CapsulePrimitive(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale)
			: RenderComponent(CapsuleVertices(), position, rotation, scale, "CapsulePrimitive") {}
		CapsulePrimitive::CapsulePrimitive(tinyxml2::XMLElement* element) : RenderComponent(element) {
		    object = Object(CapsuleVertices(), color);
		}
		FactoryBase* capsulefactory = GameObject::AddComponentFactory<CapsulePrimitive>(std::string("CapsulePrimitive"));
	}
}
#endif
