#ifdef StevEngine_RENDERER_GL
#include "Lights.hpp"
#include "visuals/renderer/RenderSystem.hpp"
#include "main/GameObject.hpp"
#include "main/Component.hpp"
#include "utilities/Vector3.hpp"

using namespace StevEngine::Renderer;

namespace StevEngine::Visuals {
	Light::Light(uint32_t shaderID, Utilities::Vector3 diffuse, Utilities::Vector3 specular, std::string type)
	  : shaderLightID(shaderID), diffuse(diffuse), specular(specular) {};
	//Create light components
	DirectionalLight::DirectionalLight(Utilities::Vector3 diffuse, Utilities::Vector3 specular)
	  : Light(Renderer::render.GetLightID("DirectionalLight"), diffuse, specular, "DirectionalLight") {
		Renderer::render.AddLight((Light*)this);
	}
	PointLight::PointLight(Utilities::Vector3 diffuse, Utilities::Vector3 specular, float constant, float linear, float quadratic)
	  : Light(Renderer::render.GetLightID("PointLight"), diffuse, specular, "PointLight"), constant(constant), linear(linear), quadratic(quadratic) {
		Renderer::render.AddLight(this);
	}
	SpotLight::SpotLight(Utilities::Vector3 diffuse, Utilities::Vector3 specular, float cutOff, float outerCutOff)
	  : Light(Renderer::render.GetLightID("SpotLight"), diffuse, specular, "SpotLight"), cutOff(cutOff), outerCutOff(outerCutOff) {
		Renderer::render.AddLight(this);
	}
	//Update shader information functions
	void DirectionalLight::UpdateShader(const ShaderProgram& program) const {
		std::string part = "directionalLights[" + std::to_string(shaderLightID) + "].";
		program.SetShaderUniform((part + "basic.diffuse").c_str(), diffuse);
		program.SetShaderUniform((part + "basic.specular").c_str(), specular);

		program.SetShaderUniform((part + "direction").c_str(), GetParent()->GetWorldRotation().Forward());
	}
	void DirectionalLight::ResetShader(const ShaderProgram& program) const {
		std::string part = "directionalLights[" + std::to_string(shaderLightID) + "].";
		program.SetShaderUniform((part + "basic.diffuse").c_str(), Utilities::Vector3());
		program.SetShaderUniform((part + "basic.specular").c_str(), Utilities::Vector3());

		program.SetShaderUniform((part + "direction").c_str(), Utilities::Vector3());
	}
	DirectionalLight::~DirectionalLight() {
		ResetShader(render.GetDefaultFragmentShaderProgram());
	}
	void PointLight::UpdateShader(const ShaderProgram& program) const {
		std::string part = "pointLights[" + std::to_string(shaderLightID) + "].";
		program.SetShaderUniform((part + "basic.diffuse").c_str(), diffuse);
		program.SetShaderUniform((part + "basic.specular").c_str(), specular);

		program.SetShaderUniform((part + "position").c_str(), GetParent()->GetWorldPosition());

		program.SetShaderUniform((part + "constant").c_str(), constant);
		program.SetShaderUniform((part + "linear").c_str(), linear);
		program.SetShaderUniform((part + "quadratic").c_str(), quadratic);
	}
	void PointLight::ResetShader(const ShaderProgram& program) const {
		std::string part = "pointLights[" + std::to_string(shaderLightID) + "].";
		program.SetShaderUniform((part + "basic.diffuse").c_str(), Utilities::Vector3());
		program.SetShaderUniform((part + "basic.specular").c_str(), Utilities::Vector3());

		program.SetShaderUniform((part + "position").c_str(), Utilities::Vector3());

		program.SetShaderUniform((part + "constant").c_str(), 0);
		program.SetShaderUniform((part + "linear").c_str(), 0);
		program.SetShaderUniform((part + "quadratic").c_str(), 0);
	}
	PointLight::~PointLight() {
		ResetShader(render.GetDefaultFragmentShaderProgram());
	}
	void SpotLight::UpdateShader(const ShaderProgram& program) const {
		std::string part = "spotLights[" + std::to_string(shaderLightID) + "].";
		program.SetShaderUniform((part + "basic.diffuse").c_str(), diffuse);
		program.SetShaderUniform((part + "basic.specular").c_str(), specular);

		program.SetShaderUniform((part + "position").c_str(), GetParent()->GetWorldPosition());
		program.SetShaderUniform((part + "position").c_str(), GetParent()->GetWorldRotation().Forward());

		program.SetShaderUniform((part + "cutOff").c_str(), cutOff);
		program.SetShaderUniform((part + "outerCutOff").c_str(), outerCutOff);
	}
	void SpotLight::ResetShader(const ShaderProgram& program) const {
		std::string part = "spotLights[" + std::to_string(shaderLightID) + "].";
		program.SetShaderUniform((part + "basic.diffuse").c_str(), Utilities::Vector3());
		program.SetShaderUniform((part + "basic.specular").c_str(), Utilities::Vector3());

		program.SetShaderUniform((part + "position").c_str(), Utilities::Vector3());
		program.SetShaderUniform((part + "direction").c_str(), Utilities::Vector3());

		program.SetShaderUniform((part + "cutOff").c_str(), 0);
		program.SetShaderUniform((part + "outerCutOff").c_str(), 0);
	}
	SpotLight::~SpotLight() {
		ResetShader(render.GetDefaultFragmentShaderProgram());
	}
	Light::~Light() {
		Renderer::render.RemoveLight(this);
	}
	//Export/Import lights
	Light::Light(Utilities::Stream& stream, std::string type) : shaderLightID(Renderer::render.GetLightID(type)) {
		stream >> diffuse >> specular;
	}
	DirectionalLight::DirectionalLight(Utilities::Stream& stream) : Light(stream, DIRECTIONAL_LIGHT_TYPE) {
		Renderer::render.AddLight(this);
	}
	Utilities::Stream DirectionalLight::Export(Utilities::StreamType type) const {
		Utilities::Stream stream(type);
		stream << diffuse << specular;
		return stream;
	}
	PointLight::PointLight(Utilities::Stream& stream) : Light(stream, POINT_LIGHT_TYPE) {
		stream >> constant >> linear >> quadratic;

		Renderer::render.AddLight(this);
	}
	Utilities::Stream PointLight::Export(Utilities::StreamType type) const {
		Utilities::Stream stream(type);
		stream << diffuse << specular;

		stream << constant << linear << quadratic;

		return stream;
	}
	SpotLight::SpotLight(Utilities::Stream& stream) : Light(stream, SPOT_LIGHT_TYPE) {
		stream >> cutOff >> outerCutOff;

		Renderer::render.AddLight(this);
	}
	Utilities::Stream SpotLight::Export(Utilities::StreamType type) const {
		Utilities::Stream stream(type);
		stream << diffuse << specular;

		stream << cutOff << outerCutOff;

		return stream;
	}
}
#endif
