#ifdef StevEngine_RENDERER_GL
#include "RenderComponent.hpp"
#include "RenderSystem.hpp"
#include "visuals/renderer/Object.hpp"
#include "visuals/shaders/Shader.hpp"
#include "utilities/Vector3.hpp"

namespace StevEngine::Renderer {
	//Constructor
	RenderComponent::RenderComponent(const Object& object)
	  : object(object) {}
	RenderComponent::RenderComponent(const Object& object, Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale)
	  : position(position), rotation(rotation), scale(scale), object(object) {}
	RenderComponent::RenderComponent(const Object& object, Utilities::Stream& stream)
	  : object(object), position(stream.Read<Utilities::Vector3>()), rotation(stream.Read<Utilities::Quaternion>()), scale(stream.Read<Utilities::Vector3>())
	{
		stream >> this->object.material;
		RenderType renderType = (RenderType)stream.Read<uint32_t>();
		this->object.SetRenderType(renderType);

		uint32_t shaderCount = stream.Read<uint32_t>();
		for(uint32_t i = 0; i < shaderCount; i++)
			AddShader(ShaderProgram(stream));
	}
	//Destructor
	RenderComponent::~RenderComponent() {
		object.material.FreeAlbedo();
		object.material.FreeNormal();
	}
	//Main draw function
	void RenderComponent::Draw(const Utilities::Matrix4& transform) {
		//New transform
		Utilities::Matrix4 trnsfm = transform * Utilities::Matrix4::FromTranslationRotationScale(position, rotation, scale);
		//Draw
		Renderer::render.DrawObject(object, trnsfm);
	}
	void RenderComponent::AddShader(ShaderProgram program) {
		object.AddShader(program);
		if(shaders.contains(program.GetType())) shaders[program.GetType()] = program;
		else shaders.insert({program.GetType(), program});
	}
	void RenderComponent::RemoveShader(ShaderType type) {
		object.RemoveShader(type);
		if(shaders.contains(type)) shaders.erase(type);
	}
	Utilities::Stream RenderComponent::Export(Utilities::StreamType type) const {
		Utilities::Stream stream(type);
		stream << position << rotation << scale << object.material << (uint32_t)object.GetRenderType();

		stream << (uint32_t)shaders.size();
		for(auto&[_, program] : shaders)
			stream << program.Export(type);

		return stream;
	}
}
#endif
