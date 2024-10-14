#include "utilities/Vector2.hpp"
#ifdef StevEngine_RENDERER_GL
#include "Primitive.hpp"
#include "visuals/render/Object.hpp"
#include "main/Log.hpp"
#include "utilities/Vector3.hpp"
#include "utilities/Vertex.hpp"
#include "utilities/Quaternion.hpp"
#include "scenes/GameObject.hpp"
#include "scenes/Component.hpp"
#include "visuals/render/RenderComponent.hpp"

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
				for(int tri = -1; tri <= 1; tri += 2) {
					for(int ver = 0; ver < 3; ver++) {
						int b = (ver == 0 ? 1 : (ver == 1 ? tri : (-1)));
						int c = (ver == 0 ? 1 : (ver == 1 ? -1 : (-tri)));
						int offset = tri < 0 ? 0 : 3;
						if(side < 2) sideVertices[offset + ver] = Vector3(a, b, c);
						else if(side < 4) sideVertices[offset + ver] = Vector3(c, a, b);
						else sideVertices[offset + ver] = Vector3(b, c, a);
					}
				}
				//Texture coordinates
				Vector2 sideUV;
				Vector2 uvFlip;
				if(side < 1)	  {
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
				//UV
				std::array<Vector2, 6> sideUVs;
				for(int i = 0; i < 6; i++) {
					float a = (side < 2 ? sideVertices[i].Z : side < 4 ? sideVertices[i].Z : sideVertices[i].X) * uvFlip.X;
					float b = (side < 2 ? sideVertices[i].Y : side < 4 ? sideVertices[i].X : sideVertices[i].Y) * uvFlip.Y;
					float u = (sideUV.X + ((a + 1) / 2)) * (textureType == COVER ? 0.25 : 1);
					float v = (sideUV.Y + ((b + 1) / 2)) * (textureType == COVER ? 0.333333333333 : 1);
					sideUVs[i] = Vector2(u, v);
				}
				//Normal
				Vector3 edge1 = sideVertices[2] - sideVertices[0];
				Vector3 edge2 = sideVertices[1] - sideVertices[0];
				Vector2 deltaUV1 = sideUVs[1] - sideUVs[0];
				Vector2 deltaUV2 = sideUVs[2] - sideUVs[0];
				Vector3 normal = Vector3::Cross(edge2, edge1).Normalized() * -a;
				double f = 1.0 / (deltaUV1.X * deltaUV2.Y - deltaUV2.X * deltaUV1.Y);
				Vector3 tangent = Vector3(
					f * (deltaUV2.Y * edge1.X - deltaUV1.Y * edge2.X),
					f * (deltaUV2.Y * edge1.Y - deltaUV1.Y * edge2.Y),
					f * (deltaUV2.Y * edge1.Z - deltaUV1.Y * edge2.Z)
				).Normalized() * a;
				//Combine to vertices
				for(int i = 0; i < 6; i++) {
					if(a == 1) 	vertices[side * 6 + i] = Vertex(sideVertices[i] * r, sideUVs[i], normal, tangent);
					else 		vertices[side * 6 + i] = Vertex(sideVertices[5 - i] * r, sideUVs[5 - i], normal, tangent);
				}
			}
			return std::vector<Vertex>(vertices.begin(), vertices.end());
		}
		Object cubeCoverObject = Object(CubeVertices(COVER));
		CubePrimitive::CubePrimitive(Vector3 position, Quaternion rotation, Vector3 scale, TextureType textureType)
			: RenderComponent(Object(cubeCoverObject), position, rotation, scale, "CubePrimitive"), textureType(textureType) {}
		CubePrimitive::CubePrimitive(YAML::Node node)
			: RenderComponent(CubeVertices((TextureType)node["textureType"].as<uint32_t>()), node) {}
		YAML::Node CubePrimitive::Export(YAML::Node node) const {
			YAML::Node n = RenderComponent::Export(node);
			n["textureType"] = (uint32_t)textureType;
			return n;
		}

		//Sphere
		static const std::vector<Vertex> SphereVertices(TextureType textureType, int detail = 20, float height = r) {
			float anglePerStep = pi2 / detail; //In Radians
			float divide = pi2;
			if(textureType == REPEAT) {
				divide /= 4;
			}
			//Calculate circle layers
			std::vector<std::vector<Vector3>> layers;
			for (int stack = -detail / 2; stack < detail / 2; stack++) {
				float stackAngle = anglePerStep * stack;
				float currentHeight = std::sin(stackAngle) * height;
				float cosStackAngle = std::cos(stackAngle);
				std::vector<Vector3> circle;
				for (int section = 0; section <= detail; section++)
				{
					float sectionAngle = (anglePerStep)*section;
					circle.emplace_back(r * std::cos(sectionAngle) * cosStackAngle, currentHeight, r * std::sin(sectionAngle) * cosStackAngle);
				}
				layers.push_back(circle);
			}
			//Create triangles
			std::vector<Vertex> vertices;
			for (int i = 0; i < layers.size() - 1; i++) {
				const std::vector<Vector3>& circle1 = layers[i];
				const std::vector<Vector3>& circle2 = layers[i + 1];
				for (int j = 0; j < circle1.size() - 1; j++) {
					//Vertices
					Vector3 a = circle1[j + 0];
					Vector3 b = circle1[j + 1];
					Vector3 c = circle2[j + 1];
					Vector3 d = circle2[j + 0];
					//UV
					Vector2 aUV = Vector2(-(0.125 + std::atan2(a.Z, a.X)/divide), -((height == r ? 0.5 : 0.75) + asin(a.Y)));
					Vector2 bUV = Vector2(-(0.125 + std::atan2(b.Z, b.X)/divide), -((height == r ? 0.5 : 0.75) + asin(b.Y)));
					Vector2 cUV = Vector2(-(0.125 + std::atan2(c.Z, c.X)/divide), -((height == r ? 0.5 : 0.75) + asin(c.Y)));
					Vector2 dUV = Vector2(-(0.125 + std::atan2(d.Z, d.X)/divide), -((height == r ? 0.5 : 0.75) + asin(d.Y)));
					//Tangent
					Vector3 edge1 = b - a;
					Vector3 edge2 = c - a;
					Vector2 deltaUV1 = bUV - aUV;
					Vector2 deltaUV2 = cUV - aUV;
					double f = 1.0 / (deltaUV1.X * deltaUV2.Y - deltaUV2.X * deltaUV1.Y);
					Vector3 tangent = Vector3(
						f * (deltaUV2.Y * edge1.X - deltaUV1.Y * edge2.X),
						f * (deltaUV2.Y * edge1.Y - deltaUV1.Y * edge2.Y),
						f * (deltaUV2.Y * edge1.Z - deltaUV1.Y * edge2.Z)
					);
					//Push
					vertices.push_back(Vertex(a, aUV, a.Normalized(), tangent));
					vertices.push_back(Vertex(b, bUV, b.Normalized(), tangent));
					vertices.push_back(Vertex(c, cUV, c.Normalized(), tangent));

					vertices.push_back(Vertex(a, aUV, a.Normalized(), tangent));
					vertices.push_back(Vertex(c, cUV, c.Normalized(), tangent));
					vertices.push_back(Vertex(d, dUV, d.Normalized(), tangent));
				}
			}
			return vertices;
		}
		SpherePrimitive::SpherePrimitive(Vector3 position, Quaternion rotation, Vector3 scale, TextureType textureType)
			: RenderComponent(SphereVertices(textureType), position, rotation, scale, "SpherePrimitive"), textureType(textureType) {}
		SpherePrimitive::SpherePrimitive(YAML::Node node)
			: RenderComponent(SphereVertices((TextureType)node["textureType"].as<uint32_t>()), node) {}
		YAML::Node SpherePrimitive::Export(YAML::Node node) const {
			YAML::Node n = RenderComponent::Export(node);
			n["textureType"] = (uint32_t)textureType;
			return n;
		}

		//Cylinder
		static const std::vector<Vertex> CylinderVertices(TextureType textureType, int detail = 20) {
			float third = 0.33333333333;
			float quarter = 0.25;
			if(textureType == REPEAT) {
				third = 1;
				quarter = 1;
			}
			//Create circle
			float anglePerStep = pi2 / detail; //In Radians
			std::vector<Vector2> circle;
			for (int i = 0; i <= detail; i++)
			{
				float currentAngle = (anglePerStep) * i;
				circle.push_back(Vector2(r * std::cos(currentAngle), r * std::sin(currentAngle)));
			}
			//Create triangles
			std::vector<Vertex> vertices;
			// Bottom and top
			for(int f = -1; f <= 1; f += 2) {
				Vector3 faceCenter = Vector3(0, f * r, 0);
				Vector3 faceNormal = Vector3::up * f;
				Vector2 faceCenterUV = Vector2(0.625, (f == -1 ? 2.5 : 0.5) * third);
				for (int i = 0; i < circle.size() - 1; i++) {
					Vector3 a = Vector3(circle[i].X, f * r, circle[i].Y);
					Vector3 b = Vector3(circle[i + 1].X, f * r, circle[i + 1].Y);
					Vector2 aUV = Vector2(faceCenterUV.X + (a.X) * quarter, faceCenterUV.Y - (a.Y) * third);
					Vector2 bUV = Vector2(faceCenterUV.X + (b.X) * quarter, faceCenterUV.Y - (b.Y) * third);
					//Tangent
					Vector3 edge1 = b - a;
					Vector3 edge2 = faceCenter - a;
					Vector2 deltaUV1 = bUV - aUV;
					Vector2 deltaUV2 = faceCenterUV - aUV;
					double f = 1.0 / (deltaUV1.X * deltaUV2.Y - deltaUV2.X * deltaUV1.Y);
					Vector3 tangent = Vector3(
						f * (deltaUV2.Y * edge1.X - deltaUV1.Y * edge2.X),
						f * (deltaUV2.Y * edge1.Y - deltaUV1.Y * edge2.Y),
						f * (deltaUV2.Y * edge1.Z - deltaUV1.Y * edge2.Z)
					);
					//Add vertices
					vertices.push_back(Vertex(faceCenter, faceCenterUV, faceNormal, tangent));
					if(f == -1) {
						vertices.push_back(Vertex(a, aUV, faceNormal, tangent));
						vertices.push_back(Vertex(b, bUV, faceNormal, tangent));
					} else {
						vertices.push_back(Vertex(b, bUV, faceNormal, tangent));
						vertices.push_back(Vertex(a, aUV, faceNormal, tangent));
					}
				}
			}
			// Sides
			float divide = pi2;
			if(textureType == REPEAT) {
				divide /= 4;
			}
			for (int i = 0; i < circle.size() - 1; i++) {
				Vector3 a = Vector3(circle[i].X, -r, circle[i].Y);
				Vector3 b = Vector3(circle[i].X, r, circle[i].Y);
				Vector3 c = Vector3(circle[i+1].X, r, circle[i+1].Y);
				Vector3 d = Vector3(circle[i+1].X, -r, circle[i+1].Y);

				Vector3 aNormal = Vector3(a.X, 0, a.Z).Normalized();
				Vector3 bNormal = Vector3(b.X, 0, b.Z).Normalized();
				Vector3 cNormal = Vector3(c.X, 0, c.Z).Normalized();
				Vector3 dNormal = Vector3(d.X, 0, d.Z).Normalized();

				Vector2 aUV = Vector2(-(std::atan2(a.Z, a.X) / divide) - 0.125, 2 * third);
				Vector2 bUV = Vector2(-(std::atan2(b.Z, b.X) / divide) - 0.125, 1 * third);
				Vector2 cUV = Vector2(-(std::atan2(c.Z, c.X) / divide) - 0.125, 1 * third);
				Vector2 dUV = Vector2(-(std::atan2(d.Z, d.X) / divide) - 0.125, 2 * third);

				Vector3 edge1 = b - a;
				Vector3 edge2 = c - a;
				Vector2 deltaUV1 = bUV - aUV;
				Vector2 deltaUV2 = cUV - aUV;
				double f = 1.0 / (deltaUV1.X * deltaUV2.Y - deltaUV2.X * deltaUV1.Y);
				Vector3 tangent = Vector3(
					f * (deltaUV2.Y * edge1.X - deltaUV1.Y * edge2.X),
					f * (deltaUV2.Y * edge1.Y - deltaUV1.Y * edge2.Y),
					f * (deltaUV2.Y * edge1.Z - deltaUV1.Y * edge2.Z)
				);

				vertices.emplace_back(a, aUV, aNormal, tangent);
				vertices.emplace_back(b, bUV, bNormal, tangent);
				vertices.emplace_back(c, cUV, cNormal, tangent);

				vertices.emplace_back(a, aUV, aNormal, tangent);
				vertices.emplace_back(c, cUV, cNormal, tangent);
				vertices.emplace_back(d, dUV, dNormal, tangent);
			}
			//Return
			return vertices;
		}
		CylinderPrimitive::CylinderPrimitive(Vector3 position, Quaternion rotation, Vector3 scale, TextureType textureType)
			: RenderComponent(CylinderVertices(textureType), position, rotation, scale, "CylinderPrimitive"), textureType(textureType) {}
		CylinderPrimitive::CylinderPrimitive(YAML::Node node)
			: RenderComponent(CylinderVertices((TextureType)node["textureType"].as<uint32_t>()), node) {}
		YAML::Node CylinderPrimitive::Export(YAML::Node node) const {
			YAML::Node n = RenderComponent::Export(node);
			n["textureType"] = (uint32_t)textureType;
			return n;
		}

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
				v.position.Y -= r;
				vertices.push_back(v);
			}
			// Top sphere
			for(int i = sphere.size() / 2; i < sphere.size(); i++) {
				Vertex v = sphere[i];
				v.position.Y += r;
				vertices.push_back(v);
			}
			// Sides
			float anglePerStep = pi2 / detail; //In Radians
			float height = r;
			std::vector<Vector2> circle;
			for (int i = 0; i <= detail; i++)
			{
				float currentAngle = (anglePerStep) * i;
				circle.push_back(Vector2(r * std::cos(currentAngle), r * std::sin(currentAngle)));
			}
			for (int i = 0; i < circle.size() - 1; i++) {
				Vector3 a = Vector3(circle[i].X, -r, circle[i].Y);
				Vector3 b = Vector3(circle[i].X, r, circle[i].Y);
				Vector3 c = Vector3(circle[i+1].X, r, circle[i+1].Y);
				Vector3 d = Vector3(circle[i+1].X, -r, circle[i+1].Y);

				Vector3 aNormal = Vector3(a.X, 0, a.Z).Normalized();
				Vector3 bNormal = Vector3(b.X, 0, b.Z).Normalized();
				Vector3 cNormal = Vector3(c.X, 0, c.Z).Normalized();
				Vector3 dNormal = Vector3(d.X, 0, d.Z).Normalized();

				Vector2 aUV = Vector2(-(std::atan2(a.Z, a.X) / divide) - 0.125, 2 * third);
				Vector2 bUV = Vector2(-(std::atan2(b.Z, b.X) / divide) - 0.125, 1 * third);
				Vector2 cUV = Vector2(-(std::atan2(c.Z, c.X) / divide) - 0.125, 1 * third);
				Vector2 dUV = Vector2(-(std::atan2(d.Z, d.X) / divide) - 0.125, 2 * third);

				Vector3 edge1 = b - a;
				Vector3 edge2 = c - a;
				Vector2 deltaUV1 = bUV - aUV;
				Vector2 deltaUV2 = cUV - aUV;
				double f = 1.0 / (deltaUV1.X * deltaUV2.Y - deltaUV2.X * deltaUV1.Y);
				Vector3 tangent = Vector3(
					f * (deltaUV2.Y * edge1.X - deltaUV1.Y * edge2.X),
					f * (deltaUV2.Y * edge1.Y - deltaUV1.Y * edge2.Y),
					f * (deltaUV2.Y * edge1.Z - deltaUV1.Y * edge2.Z)
				);

				vertices.emplace_back(a, aUV, aNormal, tangent);
				vertices.emplace_back(b, bUV, bNormal, tangent);
				vertices.emplace_back(c, cUV, cNormal, tangent);

				vertices.emplace_back(a, aUV, aNormal, tangent);
				vertices.emplace_back(c, cUV, cNormal, tangent);
				vertices.emplace_back(d, dUV, dNormal, tangent);
			}
			//Return
			return vertices;
		}
		CapsulePrimitive::CapsulePrimitive(Vector3 position, Quaternion rotation, Vector3 scale, TextureType textureType)
			: RenderComponent(CapsuleVertices(textureType), position, rotation, scale, "CapsulePrimitive"), textureType(textureType) {}
		CapsulePrimitive::CapsulePrimitive(YAML::Node node)
			: RenderComponent(CapsuleVertices((TextureType)node["textureType"].as<uint32_t>()), node) {}
		YAML::Node CapsulePrimitive::Export(YAML::Node node) const {
			YAML::Node n = RenderComponent::Export(node);
			n["textureType"] = (uint32_t)textureType;
			return n;
		}
	}
}
#endif
