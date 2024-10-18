
#ifdef StevEngine_RENDERER_GL
#include "Object.hpp"
#include "utilities/Color.hpp"
#include "utilities/Vertex.hpp"
#include "visuals/Texture.hpp"
#include "visuals/render/RenderSystem.hpp"
#include "visuals/render/Lights.hpp"
#include "visuals/Camera.hpp"
#include "scenes/SceneManager.hpp"
#include "main/Log.hpp"

#include <memory>
#include <string>
#include <cstdint>
#include <algorithm>

#include "glad/gl.h"

using StevEngine::Utilities::Vertex;
using StevEngine::Utilities::Color;

namespace StevEngine {
	namespace Render {
		std::vector<float> ToFloatList(const std::vector<Vertex>& vertices) {
			std::vector<float> result;
			result.resize(vertices.size() * (Utilities::VERTEX_COUNT));
			int j = 0;
			for(int i = 0; i < vertices.size(); i++) {
				const Vertex& vertex = vertices[i];
				result[j++] = vertex.position.X;
				result[j++] = vertex.position.Y;
				result[j++] = vertex.position.Z;
				result[j++] = vertex.uv.X;
				result[j++] = vertex.uv.Y;
				result[j++] = vertex.normal.X;
				result[j++] = vertex.normal.Y;
				result[j++] = vertex.normal.Z;
				result[j++] = vertex.tangent.X;
				result[j++] = vertex.tangent.Y;
				result[j++] = vertex.tangent.Z;
			}
			return result;
		}
		Object::Object(const std::vector<Vertex>& vertices, const Color& color, const Visuals::Texture& textureData, const Visuals::Texture& normalMapData)
	 		: texture(textureData), normalMap(normalMapData) {
			//Create indices and filter out duplicates
			std::vector<Vertex> uniqueVertices;
			std::vector<uint32_t> newIndices;
			size_t i = 0;
			for (Vertex v : vertices) {
				int index = std::find(uniqueVertices.begin(), uniqueVertices.end(), v) - uniqueVertices.begin();
				if(index == uniqueVertices.size()) {
					uniqueVertices.emplace_back(v);
				}
				newIndices.emplace_back(index);
			}
			//Fill vertex and index arrays
			auto floatVertices = ToFloatList(uniqueVertices);
			vertexCount = floatVertices.size();
			this->vertices = new float[vertexCount];
			for(int i = 0; i < vertexCount; i++) {
				this->vertices[i] = floatVertices[i];
			}
			indexCount = newIndices.size();
			this->indices = new uint32_t[indexCount];
			for(int i = 0; i < indexCount; i++) {
				this->indices[i] = newIndices[i];
			}
			//Bind texture
			SetTexture(textureData);
		}
		Object::Object(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const Utilities::Color& color, const Visuals::Texture& textureData, const Visuals::Texture& normalMapData)
			: vertices(new float[vertices.size()]), vertexCount(vertices.size()), indices(new uint32_t[indices.size()]), indexCount(indices.size())
		{
			auto floatVertices = ToFloatList(vertices);
			vertexCount = floatVertices.size();
			this->vertices = new float[vertexCount];
			for(int i = 0; i < vertexCount; i++) {
				this->vertices[i] = floatVertices[i];
			}
			indexCount = indices.size();
			this->indices = new uint32_t[indexCount];
			for(int i = 0; i < indexCount; i++) {
				this->indices[i] = indices[i];
			}
			SetTexture(textureData);
			SetTexture(normalMapData);
		}
		Object::Object(const Object& instance, const Color& color, const Visuals::Texture& textureData, const Visuals::Texture& normalMapData)
			: indices(instance.indices), indexCount(instance.indexCount), vertices(instance.vertices), vertexCount(instance.vertexCount), color(color)
		{
			SetTexture(textureData);
			SetTexture(normalMapData);
		}
		void Object::SetTexture(const Visuals::Texture& textureData) {
			texture = textureData;
			texture.BindTexture();
		}
		void Object::SetNormalMap(const Visuals::Texture& normalData) {
			normalMap = normalData;
			normalMap.BindTexture();
		}
		void Object::FreeTexture() {
			texture.FreeTexture();
		}
		void Object::FreeNormalMap() {
			normalMap.FreeTexture();
		}
		void Object::AddShader(Render::ShaderProgram program) {
			if(shaders.contains(program.GetType())) RemoveShader(program.GetType());
			program.RelinkProgram();
			shaders.insert({ program.GetType(), program });
		}
		void Object::RemoveShader(Render::ShaderType type) {
			shaders.erase(shaders.find(type));
		}

		//Draw
		void Object::Draw(glm::mat4x4 transform) const {
			//Object specific shaders
			uint32_t pipeline;
			const ShaderProgram* vertexProgram = &render.GetDefaultVertexShaderProgram();
			const ShaderProgram* fragmentProgram = &render.GetDefaultFragmentShaderProgram();
			bool usingCustomShaders = shaders.size() > 0;
			if(usingCustomShaders) {
				//Set shader programs
				glGenProgramPipelines(1, &pipeline);
				glBindProgramPipeline(pipeline);
				if(shaders.contains(VERTEX)) {
					vertexProgram = &shaders.at(VERTEX);
				}
				glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vertexProgram->GetLocation());
				if(shaders.contains(FRAGMENT)) {
					fragmentProgram = &shaders.at(FRAGMENT);
				}
				glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fragmentProgram->GetLocation());
				//Update program with basic info
				Visuals::Camera* camera = sceneManager.GetActiveScene()->GetCamera();
				//  View matrix
				vertexProgram->SetShaderUniform("viewTransform", camera->GetView());
				fragmentProgram->SetShaderUniform("viewPosition", (glm::vec3)camera->GetParent()->GetWorldPosition());
				//  Projection matrix
				vertexProgram->SetShaderUniform("projectionTransform", camera->GetProjection());
				//  Ambient lighting
				auto ambientLightColor = render.GetAmbientLightColor();
				fragmentProgram->SetShaderUniform("ambientColor", glm::vec3(ambientLightColor.r / 255.0f, ambientLightColor.g / 255.0f, ambientLightColor.b / 255.0f));
				fragmentProgram->SetShaderUniform("ambientStrength", render.GetAmbientLightStrength());
				//	Other lights
				for(auto light : render.GetLights()) {
					light->UpdateShader(*fragmentProgram);
				}
			}
			//Update transform
			vertexProgram->SetShaderUniform("objectTransform", transform);
			//Update texture
			fragmentProgram->SetShaderUniform("objectIsTextured", texture.IsBound());
			if(texture.IsBound()) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture.GetGLLocation());
				fragmentProgram->SetShaderUniform("objectTexture", 0);
			}
			//Update normal map
			fragmentProgram->SetShaderUniform("objectIsNormalMapped", normalMap.IsBound());
			if(normalMap.IsBound()) {
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, normalMap.GetGLLocation());
				fragmentProgram->SetShaderUniform("objectNormalMap", 1);
			}
			//Update color
			fragmentProgram->SetShaderUniform("objectColor", glm::vec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f));
			//Update material
			fragmentProgram->SetShaderUniform("objectMaterial.ambient", (glm::vec3)material.ambient);
			fragmentProgram->SetShaderUniform("objectMaterial.diffuse", (glm::vec3)material.diffuse);
			fragmentProgram->SetShaderUniform("objectMaterial.specular", (glm::vec3)material.specular);
			fragmentProgram->SetShaderUniform("objectMaterial.shininess", material.shininess);
			//Draw object
			glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices, GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint32_t), indices, GL_STATIC_DRAW);
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
			//Remove custom pipeline
			if(usingCustomShaders) {
				//Reset lights
				for(Light* light : render.GetLights()) {
					light->ResetShader(*fragmentProgram);
				}
				glBindProgramPipeline(render.GetShaderPipeline());
				glDeleteProgramPipelines(1, &pipeline);
			}
		}
	}
}
#endif
