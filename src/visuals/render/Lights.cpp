#ifdef StevEngine_RENDERER_GL
#include "Lights.hpp"
#include "visuals/render/RenderSystem.hpp"
#include "scenes/GameObject.hpp"
#include "scenes/Component.hpp"
#include "utilities/Vector3.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace StevEngine::Render {
	Light::Light(uint32_t shaderID, Utilities::Vector3 diffuse, Utilities::Vector3 specular, std::string type)
		: shaderLightID(shaderID), diffuse(diffuse), specular(specular), Component(type) {};
	//Create light components
	DirectionalLight::DirectionalLight(Utilities::Vector3 diffuse, Utilities::Vector3 specular)
		: Light(Render::render.GetLightID("DirectionalLight"), diffuse, specular, "DirectionalLight") {
		Render::render.lights.push_back(this);
	}
	PointLight::PointLight(Utilities::Vector3 diffuse, Utilities::Vector3 specular, float constant, float linear, float quadratic)
		: Light(Render::render.GetLightID("PointLight"), diffuse, specular, "PointLight"), constant(constant), linear(linear), quadratic(quadratic) {
		Render::render.lights.push_back(this);
	}
	SpotLight::SpotLight(Utilities::Vector3 diffuse, Utilities::Vector3 specular, float cutOff, float outerCutOff)
		: Light(Render::render.GetLightID("SpotLight"), diffuse, specular, "SpotLight"), cutOff(cutOff), outerCutOff(outerCutOff) {
		Render::render.lights.push_back(this);
	}
	//Update shader information functions
	void DirectionalLight::UpdateShader(const ShaderProgram& program) const {
		std::string part = "directionalLights[" + std::to_string(shaderLightID) + "].";
		program.SetShaderUniform((part + "basic.diffuse").c_str(), (glm::vec3)diffuse);
		program.SetShaderUniform((part + "basic.specular").c_str(), (glm::vec3)specular);

		program.SetShaderUniform((part + "direction").c_str(), (glm::vec3)GetParent()->GetWorldRotation().Forward());
	}
	void DirectionalLight::ResetShader(const ShaderProgram& program) const {
		std::string part = "directionalLights[" + std::to_string(shaderLightID) + "].";
		program.SetShaderUniform((part + "basic.diffuse").c_str(), glm::vec3());
		program.SetShaderUniform((part + "basic.specular").c_str(), glm::vec3());

		program.SetShaderUniform((part + "direction").c_str(), glm::vec3());
	}
	DirectionalLight::~DirectionalLight() {
		ResetShader(render.GetDefaultFragmentShaderProgram());
	}
	void PointLight::UpdateShader(const ShaderProgram& program) const {
		std::string part = "pointLights[" + std::to_string(shaderLightID) + "].";
		program.SetShaderUniform((part + "basic.diffuse").c_str(), (glm::vec3)diffuse);
		program.SetShaderUniform((part + "basic.specular").c_str(), (glm::vec3)specular);

		program.SetShaderUniform((part + "position").c_str(), (glm::vec3)GetParent()->GetWorldPosition());

		program.SetShaderUniform((part + "constant").c_str(), constant);
		program.SetShaderUniform((part + "linear").c_str(), linear);
		program.SetShaderUniform((part + "quadratic").c_str(), quadratic);
	}
	void PointLight::ResetShader(const ShaderProgram& program) const {
		std::string part = "pointLights[" + std::to_string(shaderLightID) + "].";
		program.SetShaderUniform((part + "basic.diffuse").c_str(), glm::vec3());
		program.SetShaderUniform((part + "basic.specular").c_str(), glm::vec3());

		program.SetShaderUniform((part + "position").c_str(), glm::vec3());

		program.SetShaderUniform((part + "constant").c_str(), 0);
		program.SetShaderUniform((part + "linear").c_str(), 0);
		program.SetShaderUniform((part + "quadratic").c_str(), 0);
	}
	PointLight::~PointLight() {
		ResetShader(render.GetDefaultFragmentShaderProgram());
	}
	void SpotLight::UpdateShader(const ShaderProgram& program) const {
		std::string part = "spotLights[" + std::to_string(shaderLightID) + "].";
		program.SetShaderUniform((part + "basic.diffuse").c_str(), (glm::vec3)diffuse);
		program.SetShaderUniform((part + "basic.specular").c_str(), (glm::vec3)specular);

		program.SetShaderUniform((part + "position").c_str(), (glm::vec3)GetParent()->GetWorldPosition());
		program.SetShaderUniform((part + "position").c_str(), (glm::vec3)GetParent()->GetWorldRotation().Forward());

		program.SetShaderUniform((part + "cutOff").c_str(), cutOff);
		program.SetShaderUniform((part + "outerCutOff").c_str(), outerCutOff);
	}
	void SpotLight::ResetShader(const ShaderProgram& program) const {
		std::string part = "spotLights[" + std::to_string(shaderLightID) + "].";
		program.SetShaderUniform((part + "basic.diffuse").c_str(), glm::vec3());
		program.SetShaderUniform((part + "basic.specular").c_str(), glm::vec3());

		program.SetShaderUniform((part + "position").c_str(), glm::vec3());
		program.SetShaderUniform((part + "direction").c_str(), glm::vec3());

		program.SetShaderUniform((part + "cutOff").c_str(), 0);
		program.SetShaderUniform((part + "outerCutOff").c_str(), 0);
	}
	SpotLight::~SpotLight() {
		ResetShader(render.GetDefaultFragmentShaderProgram());
	}
	Light::~Light() {
		Render::render.lights.erase(std::find(Render::render.lights.begin(), Render::render.lights.end(), this));
	}
	//Export/Import lights
	Light::Light(YAML::Node node) : shaderLightID(Render::render.GetLightID(node["type"].as<std::string>())), Component(node) {
		diffuse = node["diffuse"].as<Utilities::Vector3>();
		specular = node["specular"].as<Utilities::Vector3>();
	}
	DirectionalLight::DirectionalLight(YAML::Node node) : Light(node) {
		Render::render.lights.push_back(this);
	}
	YAML::Node DirectionalLight::Export(YAML::Node node) const {
		node["diffuse"] = diffuse;
		node["specular"] = specular;
		return node;
	}
	PointLight::PointLight(YAML::Node node) : Light(node) {
		constant = node["constant"].as<float>();
		linear = node["linear"].as<float>();
		quadratic = node["quadratic"].as<float>();

		Render::render.lights.push_back(this);
	}
	YAML::Node PointLight::Export(YAML::Node node) const {
		node["diffuse"] = diffuse;
		node["specular"] = specular;

		node["constant"] = constant;
		node["linear"] = linear;
		node["quadratic"] = quadratic;

		return node;
	}
	SpotLight::SpotLight(YAML::Node node) : Light(node) {
		cutOff = node["cutOff"].as<float>();
		outerCutOff = node["outerCutOff"].as<float>();

		Render::render.lights.push_back(this);
	}
	YAML::Node SpotLight::Export(YAML::Node node) const {
		node["diffuse"] = diffuse;
		node["specular"] = specular;

		node["cutOff"] = cutOff;
		node["outerCutOff"] = outerCutOff;

		return node;
	}
}
#endif
