#include <cmath>
#ifdef StevEngine_RENDERER_GL
#include "Primitive.hpp"
#include "visuals/render/Object.hpp"
#include "main/Log.hpp"
#include "utilities/Vector3.hpp"
#include "utilities/Vertex.hpp"
#include "utilities/Quaternion.hpp"
#include "scenes/GameObject.hpp"

#include <SDL.h>
#include <glad/gl.h>
#include <math.h>
#include <array>
#include <vector>

using namespace StevEngine::Render;
using namespace StevEngine::Utilities;

namespace StevEngine {
	namespace Visuals {
		float r = 0.5;
		float pi2 = (2*M_PI);
	    //Cube
		static const std::vector<Vertex> CubeVertices(TextureType textureType) {
			std::array<Vertex, 6*6> vertices;
			for(int side = 0; side < 6; side++) {
				//Coordinates
				int a = (side % 2 ? 1 : -1);
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
                //Texture coordinates
               	Vector2 sideUV;
               	Vector2 uvFlip;

               	if(side < 1)      {
                    sideUV = Vector2(1, 1);
                    uvFlip = Vector2(1, -1);
                }
               	else if(side < 2) {
                    sideUV = Vector2(3, 1);
                    uvFlip = Vector2(-1, -1);
                }
               	else if(side < 3) {
                    sideUV = Vector2(1, 2);
                    uvFlip = Vector2(1, 1);
                }
               	else if(side < 4) {
                    sideUV = Vector2(1, 0);
                    uvFlip = Vector2(1, -1);
                }
               	else if(side < 5) {
                    sideUV = Vector2(0, 1);
                    uvFlip = Vector2(-1, -1);
                }
               	else {
                    sideUV = Vector2(2, 1);
                    uvFlip = Vector2(1, -1);
                }

                if(textureType == REPEAT) {
                    sideUV = Vector2(0, 1);
                }
				//Combine to vertices
				for(int i = 0; i < 6; i++) {
                    float a = (side < 2 ? sideVertices[i].Z : side < 4 ? sideVertices[i].Z : sideVertices[i].X) * uvFlip.X;
					float b = (side < 2 ? sideVertices[i].Y : side < 4 ? sideVertices[i].X : sideVertices[i].Y) * uvFlip.Y;
					float u = (sideUV.X + ((a + 1) / 2)) * (textureType == COVER ? 0.25 : 1);
					float v = (sideUV.Y + ((b + 1) / 2)) * (textureType == COVER ? 0.333333333333 : 1);

				    vertices[side * 6 + i] = Vertex(
						sideVertices[i] * r,
						normal,
						Vector2(u, v)
					);
				}
			}
			return std::vector<Vertex>(vertices.begin(), vertices.end());
		}
		CubePrimitive::CubePrimitive(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale, TextureType textureType)
			: RenderComponent(CubeVertices(textureType), position, rotation, scale, "CubePrimitive") {}
		CubePrimitive::CubePrimitive(tinyxml2::XMLElement* element) : RenderComponent(element) {
			object = Object(CubeVertices(COVER), color);
		}
		FactoryBase* cubefactory = GameObject::AddComponentFactory<CubePrimitive>(std::string("CubePrimitive"));

		//Sphere
		static const std::vector<Vertex> SphereVertices(TextureType textureType, int detail = 30, float height = r) {
            float anglePerStep = pi2 / detail; //In Radians
            float divide = pi2;
			if(textureType == REPEAT) {
			    divide /= 4;
			}
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
					//Vertices
					Vector3 a(circle1[j + 0].X, circle1[j + 0].Y, circle1[j + 0].Z);
					Vector3 b(circle1[j + 1].X, circle1[j + 1].Y, circle1[j + 1].Z);
					Vector3 c(circle2[j + 1].X, circle2[j + 1].Y, circle2[j + 1].Z);
					Vector3 d(circle2[j + 0].X, circle2[j + 0].Y, circle2[j + 0].Z);
					//Normal
					Vector3 normal = Vector3::Cross(c - a, b - a) * -1;
					//UV
					Vector2 aUV = Vector2(-(0.125 + std::atan2(a.Z, a.X)/divide), -((height == r ? 0.5 : 0.75) + asin(a.Y)));
					Vector2 bUV = Vector2(-(0.125 + std::atan2(b.Z, b.X)/divide), -((height == r ? 0.5 : 0.75) + asin(b.Y)));
					Vector2 cUV = Vector2(-(0.125 + std::atan2(c.Z, c.X)/divide), -((height == r ? 0.5 : 0.75) + asin(c.Y)));
					Vector2 dUV = Vector2(-(0.125 + std::atan2(d.Z, d.X)/divide), -((height == r ? 0.5 : 0.75) + asin(d.Y)));

					//Push
					vertices.push_back(Vertex(a, a.Normalized(), aUV));
					vertices.push_back(Vertex(b, b.Normalized(), bUV));
					vertices.push_back(Vertex(c, c.Normalized(), cUV));

					vertices.push_back(Vertex(a, a.Normalized(), aUV));
					vertices.push_back(Vertex(c, c.Normalized(), cUV));
					vertices.push_back(Vertex(d, d.Normalized(), dUV));
				}
			}
			return vertices;
		}
		SpherePrimitive::SpherePrimitive(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale, TextureType textureType)
			: RenderComponent(SphereVertices(textureType), position, rotation, scale, "SpherePrimitive") {}
		SpherePrimitive::SpherePrimitive(tinyxml2::XMLElement* element) : RenderComponent(element) {
		    object = Object(SphereVertices(COVER), color);
		}
		FactoryBase* spherefactory = GameObject::AddComponentFactory<SpherePrimitive>(std::string("SpherePrimitive"));

		//Cylinder
		static const std::vector<Vertex> CylinderVertices(TextureType textureType, int detail = 50) {
		    float third = 0.33333333333;
			float quarter = 0.25;
			if(textureType == REPEAT) {
			    third = 1;
				quarter = 1;
			}
		    //Create circle
            float anglePerStep = pi2 / detail; //In Radians
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
			Vector3 bottomNormal = -Vector3::up;
			Vector2 bottomCenterUV = Vector2(0.625, 2.5 * third);
			for (int i = 0; i < circle.size() - 1; i++) {
			    Utilities::Vector2 a = circle[i];
			    Utilities::Vector2 b = circle[i + 1];
				vertices.push_back(Vertex(Vector3(a.X, -r, a.Y), bottomNormal, Vector2(bottomCenterUV.X + (a.X) * quarter, bottomCenterUV.Y - (a.Y) * third)));
				vertices.push_back(Vertex(Vector3(b.X, -r, b.Y), bottomNormal, Vector2(bottomCenterUV.X + (b.X) * quarter, bottomCenterUV.Y - (b.Y) * third)));
				vertices.push_back(Vertex(bottomCenter, bottomNormal, bottomCenterUV));
			}
			// Top
			Vector3 topCenter = Vector3(0, r, 0);
			Vector3 topNormal = Vector3::up;
			Vector2 topCenterUV = Vector2(0.625, 0.5 * third);
			for (int i = 0; i < circle.size() - 1; i++) {
			    Utilities::Vector2 a = circle[i];
			    Utilities::Vector2 b = circle[i + 1];
				vertices.push_back(Vertex(Vector3(a.X, r, a.Y), topNormal, Vector2(topCenterUV.X + (a.X) * quarter, topCenterUV.Y + (a.Y) * third)));
				vertices.push_back(Vertex(Vector3(b.X, r, b.Y), topNormal, Vector2(topCenterUV.X + (b.X) * quarter, topCenterUV.Y + (b.Y) * third)));
				vertices.push_back(Vertex(topCenter, topNormal, topCenterUV));
			}
			// Sides
			float divide = pi2;
			if(textureType == REPEAT) {
			    divide /= 4;
			}
			for (int i = 0; i < circle.size() - 1; i++) {
			    Utilities::Vector2 a = circle[i];
			    Utilities::Vector2 b = circle[i + 1];

				Vector3 aN = Vector3(a.X, 0, a.Y).Normalized();
				Vector3 bN = Vector3(b.X, 0, b.Y).Normalized();

				float uA = -(std::atan2(a.Y, a.X) / divide) - 0.125;
				float uB = -(std::atan2(b.Y, b.X) / divide) - 0.125;

				float vP = 1 * third;
				float vN = 2 * third;

				vertices.push_back(Vertex(Vector3(a.X, -r, a.Y), aN, Vector2(uA, vN)));
				vertices.push_back(Vertex(Vector3(b.X, r, b.Y), bN, Vector2(uB, vP)));
				vertices.push_back(Vertex(Vector3(b.X, -r, b.Y), bN, Vector2(uB, vN)));

				vertices.push_back(Vertex(Vector3(a.X, r, a.Y), aN, Vector2(uA, vP)));
				vertices.push_back(Vertex(Vector3(b.X, r, b.Y), bN, Vector2(uB, vP)));
				vertices.push_back(Vertex(Vector3(a.X, -r, a.Y), aN, Vector2(uA, vN)));
			}
			//Return
			return vertices;
		}
		CylinderPrimitive::CylinderPrimitive(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale, TextureType textureType)
			: RenderComponent(CylinderVertices(textureType), position, rotation, scale, "CylinderPrimitive") {}
		CylinderPrimitive::CylinderPrimitive(tinyxml2::XMLElement* element) : RenderComponent(element) {
		    object = Object(CylinderVertices(COVER), color);
		}
		FactoryBase* cylinderfactory = GameObject::AddComponentFactory<CylinderPrimitive>(std::string("CylinderPrimitive"));

		//Capsule
		static const std::vector<Vertex> CapsuleVertices(TextureType textureType, int detail = 30) {
    		float divide = pi2;
            float third = 0.33333333333;
			float quarter = 0.25;
    		if(textureType == REPEAT) {
    		    divide /= 4;
                third = 1;
				quarter = 1;
    		}
			//Create triangles
			std::vector<Vertex> vertices;
			// Get sphere vertices
			std::vector<Vertex> sphere = SphereVertices(textureType, detail, r/2);
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
			float anglePerStep = pi2 / detail; //In Radians
			float height = r;
            std::vector<Utilities::Vector2> circle;
           	for (int i = 0; i <= detail; i++)
           	{
          		float currentAngle = (anglePerStep) * i;
          		circle.push_back(Utilities::Vector2(r * std::cos(currentAngle), r * std::sin(currentAngle)));
           	}
			for (int i = 0; i < circle.size() - 1; i++) {
			    Utilities::Vector2 a = circle[i];
			    Utilities::Vector2 b = circle[i + 1];

				Vector3 aN = Vector3(a.X, 0, a.Y).Normalized();
				Vector3 bN = Vector3(b.X, 0, b.Y).Normalized();

				float uA = -(std::atan2(a.Y, a.X) / divide) - 0.125;
				float uB = -(std::atan2(b.Y, b.X) / divide) - 0.125;

				float vP = 1 * third;
				float vN = 2 * third;

				vertices.push_back(Vertex(Vector3(a.X, -height, a.Y), aN, Vector2(uA, vN)));
				vertices.push_back(Vertex(Vector3(b.X, height, b.Y), bN, Vector2(uB, vP)));
				vertices.push_back(Vertex(Vector3(b.X, -height, b.Y), bN, Vector2(uB, vN)));

				vertices.push_back(Vertex(Vector3(a.X, height, a.Y), aN, Vector2(uA, vP)));
				vertices.push_back(Vertex(Vector3(b.X, height, b.Y), bN, Vector2(uB, vP)));
				vertices.push_back(Vertex(Vector3(a.X, -height, a.Y), aN, Vector2(uA, vN)));
			}
			//Return
			return vertices;
		}
		CapsulePrimitive::CapsulePrimitive(Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale, TextureType textureType)
			: RenderComponent(CapsuleVertices(textureType), position, rotation, scale, "CapsulePrimitive") {}
		CapsulePrimitive::CapsulePrimitive(tinyxml2::XMLElement* element) : RenderComponent(element) {
		    object = Object(CapsuleVertices(COVER), color);
		}
		FactoryBase* capsulefactory = GameObject::AddComponentFactory<CapsulePrimitive>(std::string("CapsulePrimitive"));
	}
}
#endif
