#ifdef StevEngine_RENDERER_GL
#include "Primitive.hpp"
#include "visuals/render/Object.hpp"
#include "main/Log.hpp"
#include "utilities/Vector3.hpp"
#include "utilities/Vector2.hpp"
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
		const float r = 0.5;
		const float pi2 = (2*M_PI);
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
				Vector3 edge1, edge2;
				Vector2 deltaUV1, deltaUV2;
				if(a == 1) {
					edge1 = sideVertices[1] - sideVertices[0];
					edge2 = sideVertices[2] - sideVertices[0];
					deltaUV1 = sideUVs[1] - sideUVs[0];
					deltaUV2 = sideUVs[2] - sideUVs[0];
				} else {
					edge1 = sideVertices[4] - sideVertices[5];
					edge2 = sideVertices[3] - sideVertices[5];
					deltaUV1 = sideUVs[4] - sideUVs[5];
					deltaUV2 = sideUVs[3] - sideUVs[5];
				}
				Vector3 normal = Vector3::Cross(edge2, edge1).Normalized();
				double f = 1.0 / (deltaUV1.X * deltaUV2.Y - deltaUV2.X * deltaUV1.Y);
				Vector3 tangent = Vector3(
					f * (deltaUV2.Y * edge1.X - deltaUV1.Y * edge2.X),
					f * (deltaUV2.Y * edge1.Y - deltaUV1.Y * edge2.Y),
					f * (deltaUV2.Y * edge1.Z - deltaUV1.Y * edge2.Z)
				).Normalized();
				//Combine to vertices
				for(int i = 0; i < 6; i++) {
					if(a == 1) 	vertices[side * 6 + i] = Vertex(sideVertices[i] * r, sideUVs[i], normal, tangent);
					else 		vertices[side * 6 + i] = Vertex(sideVertices[5 - i] * r, sideUVs[5 - i], normal, tangent);
				}
			}
			return std::vector<Vertex>(vertices.begin(), vertices.end());
		}
		CubePrimitive::CubePrimitive(Vector3 position, Quaternion rotation, Vector3 scale, TextureType textureType)
			: RenderComponent(CubeVertices(textureType), position, rotation, scale, "CubePrimitive"), textureType(textureType) {}
		CubePrimitive::CubePrimitive(YAML::Node node)
			: RenderComponent(CubeVertices((TextureType)node["textureType"].as<uint32_t>()), node), textureType((TextureType)node["textureType"].as<uint32_t>()) {}
		YAML::Node CubePrimitive::Export(YAML::Node node) const {
			YAML::Node n = RenderComponent::Export(node);
			n["textureType"] = (uint32_t)textureType;
			return n;
		}

		//Sphere
		static const std::vector<Vertex> UVSphereVertices(TextureType textureType, bool smooth, int detail = 30, float height = r) {
			float anglePerStep = pi2 / detail; //In Radians
			double repeatFactor = (textureType == REPEAT ? 4 : 1);
			float divide = pi2 / repeatFactor;
			//Calculate circle layers
			std::vector<std::vector<Vector3>> layers;
			layers.reserve(detail / 2 + 1);
			for (int stack = -detail / 2.0 + 1; stack < detail / 2.0; stack++) {
				float stackAngle = (anglePerStep / 2) * stack;
				float currentHeight = std::sin(stackAngle) * height;
				float cosStackAngle = std::cos(stackAngle);
				std::vector<Vector3> circle;
				circle.reserve(detail);
				for (int section = 0; section < detail; section++)
				{
					float sectionAngle = (anglePerStep) * section;
					circle.emplace_back(r * std::cos(sectionAngle) * cosStackAngle, currentHeight, r * std::sin(sectionAngle) * cosStackAngle);
				}
				layers.push_back(circle);
			}
			//Create squares
			std::vector<Vertex> vertices;
			vertices.reserve(layers.size() * layers[0].size() * 3);
			for (int i = 0; i < layers.size() - 1; i++) {
				const std::vector<Vector3>& circle1 = layers[i];
				const std::vector<Vector3>& circle2 = layers[i + 1];
				for (int j = 0; j < detail; j++) {
					//Vertices
					Vector3 a = circle1[j];
					Vector3 b = circle1[(j < detail - 1 ? j + 1 : 0)];
					Vector3 c = circle2[(j < detail - 1 ? j + 1 : 0)];
					Vector3 d = circle2[j];
					//UV
					Vector2 aUV = Vector2((1 - ((std::atan2(a.Z, a.X) / pi2) + 0.5)) * repeatFactor, 1 - (asin(a.Y * 2) / M_PI + 0.5));
					Vector2 bUV = Vector2((1 - ((std::atan2(b.Z, b.X) / pi2) + 0.5)) * repeatFactor, 1 - (asin(b.Y * 2) / M_PI + 0.5));
					Vector2 cUV = Vector2((1 - ((std::atan2(c.Z, c.X) / pi2) + 0.5)) * repeatFactor, 1 - (asin(c.Y * 2) / M_PI + 0.5));
					Vector2 dUV = Vector2((1 - ((std::atan2(d.Z, d.X) / pi2) + 0.5)) * repeatFactor, 1 - (asin(d.Y * 2) / M_PI + 0.5));
					//Fix warped uv:
					Vector3 uvNormal = Vector3::Cross((Vector3)(bUV - aUV), (Vector3)(cUV - aUV));
					if(uvNormal.Z < 0) {
						if (aUV.X < 0.25) aUV.X += repeatFactor;
						if (bUV.X < 0.25) bUV.X += repeatFactor;
						if (cUV.X < 0.25) cUV.X += repeatFactor;
						if (dUV.X < 0.25) dUV.X += repeatFactor;
					}
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
					).Normalized();
					Vector3 faceNormal = Vector3::Cross(edge2, edge1).Normalized();
					//Create Vertices
					if(smooth) {
						vertices.emplace_back(a, aUV, a.Normalized(), tangent);
						vertices.emplace_back(b, bUV, b.Normalized(), tangent);
						vertices.emplace_back(c, cUV, c.Normalized(), tangent);

						vertices.emplace_back(a, aUV, a.Normalized(), tangent);
						vertices.emplace_back(c, cUV, c.Normalized(), tangent);
						vertices.emplace_back(d, dUV, d.Normalized(), tangent);
					} else {
						vertices.emplace_back(a, aUV, faceNormal, tangent);
						vertices.emplace_back(b, bUV, faceNormal, tangent);
						vertices.emplace_back(c, cUV, faceNormal, tangent);

						vertices.emplace_back(a, aUV, faceNormal, tangent);
						vertices.emplace_back(c, cUV, faceNormal, tangent);
						vertices.emplace_back(d, dUV, faceNormal, tangent);
					}
				}
			}
			//Create top/bottom
			for(int t = -1; t <= 1; t += 2) {
				int s = (int)(t * 0.5 + 0.5);
				Vector3 center = Vector3(0, height * t, 0);
				const std::vector<Vector3>& circle = layers[s * (layers.size() - 1)];
				for(int i = 0; i < detail; i++) {
					//Position
					Vector3 a = circle[(i < detail - 1 ? i + 1 : 0)];
					Vector3 b = circle[i];
					if(t == 1) {
						Vector3 c = a;
						a = b;
						b = c;
					}
					//UV
					Vector2 aUV = Vector2((1 - (std::atan2(a.Z, a.X) / pi2 + 0.5)) * repeatFactor, 1 - (asin(a.Y * 2) / M_PI + 0.5));
					Vector2 bUV = Vector2((1 - (std::atan2(b.Z, b.X) / pi2 + 0.5)) * repeatFactor, 1 - (asin(b.Y * 2) / M_PI + 0.5));
					Vector2 centerUV = Vector2((aUV.X + bUV.X) / 2.0, !s);
					Vector3 uvNormal = Vector3::Cross((Vector3)(bUV - aUV), (Vector3)(centerUV - aUV));
					if(uvNormal.Z < 0) {
						if (aUV.X < 0.25) aUV.X += repeatFactor;
						if (bUV.X < 0.25) bUV.X += repeatFactor;
						centerUV = Vector2((aUV.X + bUV.X) / 2.0, !s);
					}
					//Tangent
					Vector3 edge1 = b - a;
					Vector3 edge2 = center - a;
					Vector2 deltaUV1 = bUV - aUV;
					Vector2 deltaUV2 = centerUV - aUV;
					double f = 1.0 / (deltaUV1.X * deltaUV2.Y - deltaUV2.X * deltaUV1.Y);
					Vector3 tangent = Vector3(
						f * (deltaUV2.Y * edge1.X - deltaUV1.Y * edge2.X),
						f * (deltaUV2.Y * edge1.Y - deltaUV1.Y * edge2.Y),
						f * (deltaUV2.Y * edge1.Z - deltaUV1.Y * edge2.Z)
					).Normalized();
					Vector3 faceNormal = Vector3::Cross(edge2, edge1).Normalized();
					//Create Vertices
					if(smooth) {
						vertices.emplace_back(a, aUV, a.Normalized(), tangent);
						vertices.emplace_back(b, bUV, b.Normalized(), tangent);
						vertices.emplace_back(center, centerUV, center.Normalized(), tangent);
					} else {
						vertices.emplace_back(a, aUV, faceNormal, tangent);
						vertices.emplace_back(b, bUV, faceNormal, tangent);
						vertices.emplace_back(center, centerUV, faceNormal, tangent);
					}
				}
			}
			return vertices;
		}
		UVSpherePrimitive::UVSpherePrimitive(Vector3 position, Quaternion rotation, Vector3 scale, bool smooth, TextureType textureType)
			: RenderComponent(UVSphereVertices(textureType, smooth), position, rotation, scale, "UVSpherePrimitive"), textureType(textureType), smooth(smooth) {}
		UVSpherePrimitive::UVSpherePrimitive(YAML::Node node)
			: RenderComponent(UVSphereVertices((TextureType)node["textureType"].as<uint32_t>(), node["smooth"].as<bool>()), node), textureType((TextureType)node["textureType"].as<uint32_t>()), smooth(node["smooth"].as<bool>()) {}
		YAML::Node UVSpherePrimitive::Export(YAML::Node node) const {
			YAML::Node n = RenderComponent::Export(node);
			n["textureType"] = (uint32_t)textureType;
			n["smooth"] = smooth;
			return n;
		}
		//Icosphere
		/**
		* The following two functions are made on the basis of this blog: https://blog.matheusamazonas.net/posts/generating_icosphere_with_code
		*/
		static const std::array<Vector3, 20 * 3> Icosahedron() {
			std::array<Vector3, 12> icosahedronVertices;
			double goldenRatio = std::numbers::phi_v<double>;
			double a = sqrt(pow(r, 2) / (1 + pow(goldenRatio, 2)));
			double c = a * goldenRatio;
			icosahedronVertices[0 + 0] = Vector3(-a, 0,  c);
			icosahedronVertices[0 + 1] = Vector3( a, 0,  c);
			icosahedronVertices[0 + 2] = Vector3( a, 0, -c);
			icosahedronVertices[0 + 3] = Vector3(-a, 0, -c);
			icosahedronVertices[4 + 0] = Vector3( c, -a, 0);
			icosahedronVertices[4 + 1] = Vector3( c,  a, 0);
			icosahedronVertices[4 + 2] = Vector3(-c,  a, 0);
			icosahedronVertices[4 + 3] = Vector3(-c, -a, 0);
			icosahedronVertices[8 + 0] = Vector3(0,  c, -a);
			icosahedronVertices[8 + 1] = Vector3(0,  c,  a);
			icosahedronVertices[8 + 2] = Vector3(0, -c,  a);
			icosahedronVertices[8 + 3] = Vector3(0, -c, -a);
			std::array<uint32_t, 20 * 3> icosahedronIndices;
			int i = 0;
			//Edges
			for(int s = 0; s < 3; s++) {
				int s2 = s + 2;
				if(s2 >= 3) s2 -= 3;
				icosahedronIndices[i++] = s * 4 + 1;
				icosahedronIndices[i++] = s * 4 + 0;
				icosahedronIndices[i++] = s2 * 4 + 1;

				icosahedronIndices[i++] = s * 4 + 0;
				icosahedronIndices[i++] = s * 4 + 1;
				icosahedronIndices[i++] = s2 * 4 + 2;

				icosahedronIndices[i++] = s * 4 + 3;
				icosahedronIndices[i++] = s * 4 + 2;
				icosahedronIndices[i++] = s2 * 4 + 0;

				icosahedronIndices[i++] = s * 4 + 2;
				icosahedronIndices[i++] = s * 4 + 3;
				icosahedronIndices[i++] = s2 * 4 + 3;
			}
			//Corners
			{
				icosahedronIndices[i++] = 0 * 4 + 0;
				icosahedronIndices[i++] = 1 * 4 + 2;
				icosahedronIndices[i++] = 2 * 4 + 1;

				icosahedronIndices[i++] = 0 * 4 + 0;
				icosahedronIndices[i++] = 2 * 4 + 2;
				icosahedronIndices[i++] = 1 * 4 + 3;

				icosahedronIndices[i++] = 0 * 4 + 1;
				icosahedronIndices[i++] = 2 * 4 + 1;
				icosahedronIndices[i++] = 1 * 4 + 1;

				icosahedronIndices[i++] = 0 * 4 + 1;
				icosahedronIndices[i++] = 1 * 4 + 0;
				icosahedronIndices[i++] = 2 * 4 + 2;

				icosahedronIndices[i++] = 0 * 4 + 2;
				icosahedronIndices[i++] = 1 * 4 + 1;
				icosahedronIndices[i++] = 2 * 4 + 0;

				icosahedronIndices[i++] = 0 * 4 + 2;
				icosahedronIndices[i++] = 2 * 4 + 3;
				icosahedronIndices[i++] = 1 * 4 + 0;

				icosahedronIndices[i++] = 0 * 4 + 3;
				icosahedronIndices[i++] = 2 * 4 + 0;
				icosahedronIndices[i++] = 1 * 4 + 2;

				icosahedronIndices[i++] = 0 * 4 + 3;
				icosahedronIndices[i++] = 1 * 4 + 3;
				icosahedronIndices[i++] = 2 * 4 + 3;
			}
			//Create vertices
			std::array<Vector3, 20 * 3> vertices;
			int v = 0;
			for(uint32_t index : icosahedronIndices) {
				vertices[v++] = icosahedronVertices[index];
			}
			return vertices;
		}
		static const std::vector<Vertex> IcosphereVertices(TextureType textureType, bool smooth, int detail = 3) {
			//Get Icosahedron
			std::array<Vector3, 20 * 3> icosahedron = Icosahedron();

			//Fragment from detail level
			std::vector<Vector3> sphere;
			sphere.resize(icosahedron.size());
			std::copy(icosahedron.begin(), icosahedron.end(), sphere.begin());
			for(int d = 0; d < detail; d++) {
				std::vector<Vector3> newSphere;
				newSphere.reserve(sphere.size() * 4);
				for(int i = 0; i < sphere.size(); i += 3) {
					Vector3 v1 = sphere[i + 0];
					Vector3 v2 = sphere[i + 1];
					Vector3 v3 = sphere[i + 2];

					Vector3 v4 = Vector3::Slerp(v1, v2, 0.5);
					Vector3 v5 = Vector3::Slerp(v2, v3, 0.5);
					Vector3 v6 = Vector3::Slerp(v3, v1, 0.5);

					newSphere.emplace_back(v1);
					newSphere.emplace_back(v4);
					newSphere.emplace_back(v6);

					newSphere.emplace_back(v4);
					newSphere.emplace_back(v5);
					newSphere.emplace_back(v6);

					newSphere.emplace_back(v5);
					newSphere.emplace_back(v3);
					newSphere.emplace_back(v6);

					newSphere.emplace_back(v4);
					newSphere.emplace_back(v2);
					newSphere.emplace_back(v5);
				}
				sphere = newSphere;
			}
			//Transform to Vertex
			double repeatFactor = (textureType == REPEAT ? 4 : 1);
			std::vector<Vertex> vertices;
			vertices.reserve(sphere.size());
			for(int i = 0; i < sphere.size(); i += 3) {
				//Positions
				Vector3 a = sphere[i + 0];
				Vector3 b = sphere[i + 1];
				Vector3 c = sphere[i + 2];
				//UV
				Vector2 aUV = Vector2((1 - (std::atan2(a.Z, a.X) / pi2 + 0.5)) * repeatFactor, 1 - (asin(a.Y * 2) / M_PI + 0.5));
				Vector2 bUV = Vector2((1 - (std::atan2(b.Z, b.X) / pi2 + 0.5)) * repeatFactor, 1 - (asin(b.Y * 2) / M_PI + 0.5));
				Vector2 cUV = Vector2((1 - (std::atan2(c.Z, c.X) / pi2 + 0.5)) * repeatFactor, 1 - (asin(c.Y * 2) / M_PI + 0.5));
				//Fix warped uv:
				Vector3 uvNormal = Vector3::Cross((Vector3)(bUV - aUV), (Vector3)(cUV - aUV));
				if(uvNormal.Z < 0) {
					if (aUV.X < 0.25) aUV.X += repeatFactor;
					if (bUV.X < 0.25) bUV.X += repeatFactor;
					if (cUV.X < 0.25) cUV.X += repeatFactor;
				}
				//Fix UV at poles
				double top = sqrt(pow(r, 2) / (1 + pow(std::numbers::phi_v<double>, 2))) * std::numbers::phi_v<double>;
				if(aUV.Y == 0 || aUV.Y == 1) {
					aUV.X = (bUV.X + cUV.X) / 2.0;
				}
				if(bUV.Y == 0 || bUV.Y == 1) {
					bUV.X = (cUV.X + aUV.X) / 2.0;
				}
				if(cUV.Y == 0 || cUV.Y == 1) {
					cUV.X = (aUV.X + bUV.X) / 2.0;
				}
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
				).Normalized();
				Vector3 faceNormal = Vector3::Cross(edge2, edge1).Normalized();
				//Create Vertices
				if(smooth) {
					vertices.emplace_back(a, aUV, a.Normalized(), tangent);
					vertices.emplace_back(b, bUV, b.Normalized(), tangent);
					vertices.emplace_back(c, cUV, c.Normalized(), tangent);
				} else {
					vertices.emplace_back(a, aUV, faceNormal, tangent);
					vertices.emplace_back(b, bUV, faceNormal, tangent);
					vertices.emplace_back(c, cUV, faceNormal, tangent);
				}
			}
			return vertices;
		}
		IcospherePrimitive::IcospherePrimitive(Vector3 position, Quaternion rotation, Vector3 scale, bool smooth, TextureType textureType)
			: RenderComponent(IcosphereVertices(textureType, smooth), position, rotation, scale, "UVSpherePrimitive"), textureType(textureType), smooth(smooth) {}
		IcospherePrimitive::IcospherePrimitive(YAML::Node node)
			: RenderComponent(IcosphereVertices((TextureType)node["textureType"].as<uint32_t>(), node["smooth"].as<bool>()), node), textureType((TextureType)node["textureType"].as<uint32_t>()), smooth(node["smooth"].as<bool>()) {}
		YAML::Node IcospherePrimitive::Export(YAML::Node node) const {
			YAML::Node n = RenderComponent::Export(node);
			n["textureType"] = (uint32_t)textureType;
			n["smooth"] = smooth;
			return n;
		}

		//Cylinder
		static const std::vector<Vertex> CylinderVertices(TextureType textureType, bool smooth, int detail = 30, double height = r) {
			float third = 0.33333333333;
			float quarter = 0.25;
			int repeatFactor = (textureType == REPEAT ? 4 : 1);
			if(textureType == REPEAT) {
				third = 1;
				quarter = 1;
			}
			//Create circle
			float anglePerStep = pi2 / detail; //In Radians
			std::vector<Vector2> circle;
			circle.reserve(detail);
			for (int i = 0; i <= detail; i++)
			{
				float currentAngle = (anglePerStep) * i;
				circle.emplace_back(r * std::cos(currentAngle), r * std::sin(currentAngle));
			}
			//Create triangles
			std::vector<Vertex> vertices;
			// Bottom and top
			for(int f = -1; f <= 1; f += 2) {
				Vector3 faceCenter = Vector3(0, f * r, 0);
				Vector3 faceNormal = (Vector3::up * f).Normalized();
				double faceCenterV = !(f * 0.5 + 0.5);
				for (int i = 0; i < circle.size() - 1; i++) {
					Vector3 a = Vector3(circle[i].X, f * height, circle[i].Y);
					Vector3 b = Vector3(circle[i + 1].X, f * height, circle[i + 1].Y);
					if(f == -1) {
						Vector3 c = a;
						a = b;
						b = c;
					}
					//UV
					Vector2 aUV = Vector2((1 - (std::atan2(a.Z, a.X) / pi2 + 0.5)) * repeatFactor, faceCenterV ? 1 - third : third);
					Vector2 bUV = Vector2((1 - (std::atan2(b.Z, b.X) / pi2 + 0.5)) * repeatFactor, faceCenterV ? 1 - third : third);
					Vector2 centerUV = Vector2((aUV.X + bUV.X) / 2.0, faceCenterV);
					Vector3 uvNormal = Vector3::Cross((Vector3)(bUV - aUV), (Vector3)(centerUV - aUV));
					if(uvNormal.Z < 0) {
						if (aUV.X < 0.25) aUV.X += repeatFactor;
						if (bUV.X < 0.25) bUV.X += repeatFactor;
						centerUV = Vector2((aUV.X + bUV.X) / 2.0, faceCenterV);
					}
					//Tangent
					Vector3 edge1 = b - a;
					Vector3 edge2 = faceCenter - a;
					Vector2 deltaUV1 = bUV - aUV;
					Vector2 deltaUV2 = centerUV - aUV;
					double f = 1.0 / (deltaUV1.X * deltaUV2.Y - deltaUV2.X * deltaUV1.Y);
					Vector3 tangent = Vector3(
						f * (deltaUV2.Y * edge1.X - deltaUV1.Y * edge2.X),
						f * (deltaUV2.Y * edge1.Y - deltaUV1.Y * edge2.Y),
						f * (deltaUV2.Y * edge1.Z - deltaUV1.Y * edge2.Z)
					).Normalized();
					//Add vertices
					vertices.emplace_back(a, aUV, faceNormal, tangent);
					vertices.emplace_back(b, bUV, faceNormal, tangent);
					vertices.emplace_back(faceCenter, centerUV, faceNormal, tangent);
				}
			}
			// Sides
			for (int i = 0; i < circle.size() - 1; i++) {
				//Position
				Vector3 a = Vector3(circle[i].X, height, circle[i].Y);
				Vector3 b = Vector3(circle[i].X, -height, circle[i].Y);
				Vector3 c = Vector3(circle[i+1].X, -height, circle[i+1].Y);
				Vector3 d = Vector3(circle[i+1].X, height, circle[i+1].Y);
				//UV
				Vector2 aUV = Vector2((1 - (std::atan2(a.Z, a.X) / pi2 + 0.5)) * repeatFactor, 1 * third);
				Vector2 bUV = Vector2((1 - (std::atan2(b.Z, b.X) / pi2 + 0.5)) * repeatFactor, 2 * third);
				Vector2 cUV = Vector2((1 - (std::atan2(c.Z, c.X) / pi2 + 0.5)) * repeatFactor, 2 * third);
				Vector2 dUV = Vector2((1 - (std::atan2(d.Z, d.X) / pi2 + 0.5)) * repeatFactor, 1 * third);
				Vector3 uvNormal = Vector3::Cross((Vector3)(bUV - aUV), (Vector3)(cUV - aUV));
				if(uvNormal.Z < 0) {
					if (aUV.X < 0.25) aUV.X += repeatFactor;
					if (bUV.X < 0.25) bUV.X += repeatFactor;
					if (cUV.X < 0.25) cUV.X += repeatFactor;
					if (dUV.X < 0.25) dUV.X += repeatFactor;
				}
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
				).Normalized();
				//Create Vertices
				if(smooth) {
					Vector3 aNormal = Vector3(a.X, 0, a.Z).Normalized();
					Vector3 bNormal = Vector3(b.X, 0, b.Z).Normalized();
					Vector3 cNormal = Vector3(c.X, 0, c.Z).Normalized();
					Vector3 dNormal = Vector3(d.X, 0, d.Z).Normalized();
					vertices.emplace_back(a, aUV, aNormal, tangent);
					vertices.emplace_back(b, bUV, bNormal, tangent);
					vertices.emplace_back(c, cUV, cNormal, tangent);

					vertices.emplace_back(a, aUV, aNormal, tangent);
					vertices.emplace_back(c, cUV, cNormal, tangent);
					vertices.emplace_back(d, dUV, dNormal, tangent);
				} else {
					Vector3 faceNormal = Vector3::Cross(edge2, edge1).Normalized();
					vertices.emplace_back(a, aUV, faceNormal, tangent);
					vertices.emplace_back(b, bUV, faceNormal, tangent);
					vertices.emplace_back(c, cUV, faceNormal, tangent);

					vertices.emplace_back(a, aUV, faceNormal, tangent);
					vertices.emplace_back(c, cUV, faceNormal, tangent);
					vertices.emplace_back(d, dUV, faceNormal, tangent);
				}
			}
			//Return
			return vertices;
		}
		CylinderPrimitive::CylinderPrimitive(Vector3 position, Quaternion rotation, Vector3 scale, bool smooth, TextureType textureType)
			: RenderComponent(CylinderVertices(textureType, smooth), position, rotation, scale, "CylinderPrimitive"), textureType(textureType), smooth(smooth) {}
		CylinderPrimitive::CylinderPrimitive(YAML::Node node)
			: RenderComponent(CylinderVertices((TextureType)node["textureType"].as<uint32_t>(), node["smooth"].as<bool>()), node), textureType((TextureType)node["textureType"].as<uint32_t>()), smooth(node["smooth"].as<bool>()) {}
		YAML::Node CylinderPrimitive::Export(YAML::Node node) const {
			YAML::Node n = RenderComponent::Export(node);
			n["textureType"] = (uint32_t)textureType;
			n["smooth"] = smooth;
			return n;
		}

		//Capsule
		static const std::vector<Vertex> CapsuleVertices(TextureType textureType, bool smooth, int detail = 30) {
			float third = 0.33333333333;
			float quarter = 0.25;
			int repeatFactor = (textureType == REPEAT ? 4 : 1);
			if(textureType == REPEAT) {
				third = 1;
				quarter = 1;
			}
			//Create triangles
			std::vector<Vertex> vertices;
			// Get sphere vertices
			std::vector<Vertex> sphere = UVSphereVertices(textureType, smooth, detail, r / 2.0);
			for(int i = 0; i < sphere.size(); i += 3) {
				Vertex v1 = sphere[i+0];
				Vertex v2 = sphere[i+1];
				Vertex v3 = sphere[i+2];
				if(v1.position.Y < 0 || v2.position.Y < 0 || v3.position.Y < 0) {
					v1.position.Y -= r / 2;
					v2.position.Y -= r / 2;
					v3.position.Y -= r / 2;
					//UV
					v1.uv.Y = (v1.uv.Y - 0.5) * 2.0/3.0 + 2.0/3.0;
					v2.uv.Y = (v2.uv.Y - 0.5) * 2.0/3.0 + 2.0/3.0;
					v3.uv.Y = (v3.uv.Y - 0.5) * 2.0/3.0 + 2.0/3.0;
				}
				else if(v1.position.Y > 0 || v2.position.Y > 0 || v3.position.Y > 0) {
					v1.position.Y += r / 2;
					v2.position.Y += r / 2;
					v3.position.Y += r / 2;
					//UV
					v1.uv.Y *= 2.0/3.0;
					v2.uv.Y *= 2.0/3.0;
					v3.uv.Y *= 2.0/3.0;
				}
				vertices.push_back(v1);
				vertices.push_back(v2);
				vertices.push_back(v3);
			}
			for(int i = 0; i < vertices.size(); i += 3) {
				Vertex& a = vertices[i+0];
				Vertex& b = vertices[i+1];
				Vertex& c = vertices[i+2];

			}
			// Get cylinder vertices
			std::vector<Vertex> cylinder = CylinderVertices(textureType, smooth, detail, r / 2.0);
			for(int i = 0; i < cylinder.size(); i++) {
				const Vertex& v = cylinder[i];
				if(v.normal.Y < 0.1 && v.normal.Y > -0.1) {
					vertices.push_back(v);
				}
			}
			//Return
			return vertices;
		}
		CapsulePrimitive::CapsulePrimitive(Vector3 position, Quaternion rotation, Vector3 scale, bool smooth, TextureType textureType)
			: RenderComponent(CapsuleVertices(textureType, smooth), position, rotation, scale, "CapsulePrimitive"), textureType(textureType), smooth(smooth) {}
		CapsulePrimitive::CapsulePrimitive(YAML::Node node)
			: RenderComponent(CapsuleVertices((TextureType)node["textureType"].as<uint32_t>(), node["smooth"].as<bool>()), node), textureType((TextureType)node["textureType"].as<uint32_t>()), smooth(node["smooth"].as<bool>()) {}
		YAML::Node CapsulePrimitive::Export(YAML::Node node) const {
			YAML::Node n = RenderComponent::Export(node);
			n["textureType"] = (uint32_t)textureType;
			n["smooth"] = smooth;
			return n;
		}
	}
}
#endif
