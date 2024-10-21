#ifdef StevEngine_RENDERER_GL
#include "Object.hpp"
#include "utilities/Vertex.hpp"
#include "visuals/Texture.hpp"
#include "visuals/Material.hpp"
#include "visuals/renderer/RenderSystem.hpp"
#include "visuals/Lights.hpp"
#include "visuals/Camera.hpp"
#include "main/SceneManager.hpp"
#include "main/Log.hpp"

#include <memory>
#include <string>
#include <cstdint>
#include <algorithm>

#include "glad/gl.h"

using StevEngine::Utilities::Vertex;
using namespace StevEngine::Visuals;

namespace StevEngine::Renderer {
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
	Object::Object(const std::vector<Vertex>& vertices, const Visuals::Material& material)
		: material(material) {
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
	}
	Object::Object(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,  const Visuals::Material& material)
		: vertices(new float[vertices.size()]), vertexCount(vertices.size()), indices(new uint32_t[indices.size()]), indexCount(indices.size()), material(material)
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
	}
	Object::Object(const Object& instance)
		: indices(instance.indices), indexCount(instance.indexCount), vertices(instance.vertices), vertexCount(instance.vertexCount), material(instance.material) {}

	//Shaders
	void Object::AddShader(Renderer::ShaderProgram program) {
		if(shaders.contains(program.GetType())) RemoveShader(program.GetType());
		program.RelinkProgram();
		shaders.insert({ program.GetType(), program });
	}
	void Object::RemoveShader(Renderer::ShaderType type) {
		shaders.erase(shaders.find(type));
	}

	//Draw
	void Object::Draw(Utilities::Matrix4 transform) const {
		//Object specific shaders
		uint32_t pipeline;
		const ShaderProgram* vertexProgram = &render.GetDefaultVertexShaderProgram();
		const ShaderProgram* fragmentProgram = &render.GetDefaultFragmentShaderProgram();
		bool usingCustomShaders = shaders.size() > 0;
		bool usingCustomFragmentShader = false;
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
				usingCustomFragmentShader = true;
			}
			glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fragmentProgram->GetLocation());
			//Update program with basic info
			Visuals::Camera* camera = sceneManager.GetActiveScene()->GetCamera();
			//  View matrix
			vertexProgram->SetShaderUniform("viewTransform", camera->GetView());
			fragmentProgram->SetShaderUniform("viewPosition", camera->GetParent()->GetWorldPosition());
			fragmentProgram->SetShaderUniform("viewDirection", camera->GetParent()->GetWorldRotation().Forward());
			//  Projection matrix
			vertexProgram->SetShaderUniform("projectionTransform", camera->GetProjection());
			//  Ambient lighting
			fragmentProgram->SetShaderUniform("ambientColor", render.GetAmbientLightColor());
			fragmentProgram->SetShaderUniform("ambientStrength", render.GetAmbientLightStrength());
			//	Other lights
			for(auto light : render.GetLights()) {
				light->UpdateShader(*fragmentProgram);
			}
		}
		//Update transform
		vertexProgram->SetShaderUniform("objectTransform", transform);
		//Update texture
		const Visuals::Texture& albedo = material.GetAlbedo();
		bool isAlbedoTextured = albedo.IsBound();
		fragmentProgram->SetShaderUniform("usingAlbedoTexture", isAlbedoTextured);
		if(isAlbedoTextured) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, albedo.GetGLLocation());
			fragmentProgram->SetShaderUniform("albedoTexture", 0);
		}
		//Update normal map
		const Visuals::Texture& normalMap = material.GetNormal();
		bool isNormalTextured = normalMap.IsBound();
		fragmentProgram->SetShaderUniform("usingNormalTexture", isNormalTextured);
		if(isNormalTextured) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, normalMap.GetGLLocation());
			fragmentProgram->SetShaderUniform("normalTexture", 1);
		}
		//Update color
		fragmentProgram->SetShaderUniform("objectMaterial.color", material.color);
		//Update material
		fragmentProgram->SetShaderUniform("objectMaterial.ambient", material.ambient);
		fragmentProgram->SetShaderUniform("objectMaterial.diffuse", material.diffuse);
		fragmentProgram->SetShaderUniform("objectMaterial.specular", material.specular);
		fragmentProgram->SetShaderUniform("objectMaterial.shininess", material.shininess);
		//Draw object
		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint32_t), indices, GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		//Remove custom pipeline
		if(usingCustomShaders) {
			//Reset lights
			if(usingCustomFragmentShader) {
				for(Light* light : render.GetLights()) {
					light->ResetShader(*fragmentProgram);
				}
			}
			glBindProgramPipeline(render.GetShaderPipeline());
			glDeleteProgramPipelines(1, &pipeline);
		}
	}
}
#endif