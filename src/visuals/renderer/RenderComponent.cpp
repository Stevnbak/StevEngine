#ifdef StevEngine_RENDERER_GL
#include "RenderComponent.hpp"
#include "RenderSystem.hpp"
#include "visuals/shaders/Shader.hpp"
#include "utilities/Vector3.hpp"

namespace StevEngine::Renderer {
	//Constructor
	RenderComponent::RenderComponent(const Object& object)
	  : object(object) {}
	RenderComponent::RenderComponent(const Object& object, Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale)
	  : position(position), rotation(rotation), scale(scale), object(object) {}
	RenderComponent::RenderComponent(const Object& object, Stream& stream)
	  : object(object), position(stream.Read<Utilities::Vector3>()), rotation(stream.Read<Utilities::Quaternion>()), scale(stream.Read<Utilities::Vector3>())
	{
		stream >> this->object.material;
		uint shaderCount = stream.Read<uint>();
		for(int i = 0; i < shaderCount; i++)
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
		Utilities::Matrix4 trnsfm = Utilities::Matrix4::FromTranslationRotationScale(position, rotation, scale) * transform;
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
	Stream RenderComponent::Export(StreamType type) const {
		Stream stream(type);
		stream << position << rotation << scale << object.material;

		stream << (uint)shaders.size();
		for(auto&[_, program] : shaders)
			stream << program.Export(type);

		return stream;
	}
}
#endif
