#include "scenes/Component.hpp"
#ifdef StevEngine_RENDERER_GL
#include "Lights.hpp"
#include "visuals/render/System.hpp"
#include "main/Engine.hpp"
#include "scenes/GameObject.hpp"
#include "utilities/Vector3.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace StevEngine::Render {
	Light::Light(unsigned int shaderID, Utilities::Vector3 diffuse, Utilities::Vector3 specular, std::string type)
		: shaderLightID(shaderID), diffuse(diffuse), specular(specular), Component(type) {};
	//Create light components
	DirectionalLight::DirectionalLight(Utilities::Vector3 diffuse, Utilities::Vector3 specular)
		: Light(engine->render.GetLightID("DirectionalLight"), diffuse, specular, "DirectionalLight") {
		engine->render.lights.push_back(this);
	}
	PointLight::PointLight(Utilities::Vector3 diffuse, Utilities::Vector3 specular, float constant, float linear, float quadratic)
		: Light(engine->render.GetLightID("PointLight"), diffuse, specular, "PointLight"), constant(constant), linear(linear), quadratic(quadratic) {
		engine->render.lights.push_back(this);
	}
	SpotLight::SpotLight(Utilities::Vector3 diffuse, Utilities::Vector3 specular, float cutOff, float outerCutOff)
		: Light(engine->render.GetLightID("SpotLight"), diffuse, specular, "SpotLight"), cutOff(cutOff), outerCutOff(outerCutOff) {
		engine->render.lights.push_back(this);
	}
	//Update shader information functions
	void DirectionalLight::UpdateShader() {
		std::string part = "directionalLights[" + std::to_string(shaderLightID) + "].";
		engine->render.fragmentShaderProgram.SetShaderUniform((part + "basic.diffuse").c_str(), (glm::vec3)diffuse);
		engine->render.fragmentShaderProgram.SetShaderUniform((part + "basic.specular").c_str(), (glm::vec3)specular);

		engine->render.fragmentShaderProgram.SetShaderUniform((part + "direction").c_str(), (glm::vec3)GetParent()->GetWorldRotation().Forward());
	}
	DirectionalLight::~DirectionalLight() {
		std::string part = "directionalLights[" + std::to_string(shaderLightID) + "].";
		engine->render.fragmentShaderProgram.SetShaderUniform((part + "basic.diffuse").c_str(), glm::vec3());
		engine->render.fragmentShaderProgram.SetShaderUniform((part + "basic.specular").c_str(), glm::vec3());

		engine->render.fragmentShaderProgram.SetShaderUniform((part + "direction").c_str(), glm::vec3());
	}
	void PointLight::UpdateShader() {
		std::string part = "pointLights[" + std::to_string(shaderLightID) + "].";
		engine->render.fragmentShaderProgram.SetShaderUniform((part + "basic.diffuse").c_str(), (glm::vec3)diffuse);
		engine->render.fragmentShaderProgram.SetShaderUniform((part + "basic.specular").c_str(), (glm::vec3)specular);

		engine->render.fragmentShaderProgram.SetShaderUniform((part + "position").c_str(), (glm::vec3)GetParent()->GetWorldPosition());

		engine->render.fragmentShaderProgram.SetShaderUniform((part + "constant").c_str(), constant);
		engine->render.fragmentShaderProgram.SetShaderUniform((part + "linear").c_str(), linear);
		engine->render.fragmentShaderProgram.SetShaderUniform((part + "quadratic").c_str(), quadratic);
	}
	PointLight::~PointLight() {
		std::string part = "pointLights[" + std::to_string(shaderLightID) + "].";
		engine->render.fragmentShaderProgram.SetShaderUniform((part + "basic.diffuse").c_str(), glm::vec3());
		engine->render.fragmentShaderProgram.SetShaderUniform((part + "basic.specular").c_str(), glm::vec3());

		engine->render.fragmentShaderProgram.SetShaderUniform((part + "position").c_str(), glm::vec3());

		engine->render.fragmentShaderProgram.SetShaderUniform((part + "constant").c_str(), 0);
		engine->render.fragmentShaderProgram.SetShaderUniform((part + "linear").c_str(), 0);
		engine->render.fragmentShaderProgram.SetShaderUniform((part + "quadratic").c_str(), 0);
	}
	void SpotLight::UpdateShader() {
		std::string part = "spotLights[" + std::to_string(shaderLightID) + "].";
		engine->render.fragmentShaderProgram.SetShaderUniform((part + "basic.diffuse").c_str(), (glm::vec3)diffuse);
		engine->render.fragmentShaderProgram.SetShaderUniform((part + "basic.specular").c_str(), (glm::vec3)specular);

		engine->render.fragmentShaderProgram.SetShaderUniform((part + "position").c_str(), (glm::vec3)GetParent()->GetWorldPosition());
		engine->render.fragmentShaderProgram.SetShaderUniform((part + "position").c_str(), (glm::vec3)GetParent()->GetWorldRotation().Forward());

		engine->render.fragmentShaderProgram.SetShaderUniform((part + "cutOff").c_str(), cutOff);
		engine->render.fragmentShaderProgram.SetShaderUniform((part + "outerCutOff").c_str(), outerCutOff);
	}
	SpotLight::~SpotLight() {
		std::string part = "spotLights[" + std::to_string(shaderLightID) + "].";
		engine->render.fragmentShaderProgram.SetShaderUniform((part + "basic.diffuse").c_str(), glm::vec3());
		engine->render.fragmentShaderProgram.SetShaderUniform((part + "basic.specular").c_str(), glm::vec3());

		engine->render.fragmentShaderProgram.SetShaderUniform((part + "position").c_str(), glm::vec3());
		engine->render.fragmentShaderProgram.SetShaderUniform((part + "direction").c_str(), glm::vec3());

		engine->render.fragmentShaderProgram.SetShaderUniform((part + "cutOff").c_str(), 0);
		engine->render.fragmentShaderProgram.SetShaderUniform((part + "outerCutOff").c_str(), 0);
	}
	Light::~Light() {
		engine->render.lights.erase(std::find(engine->render.lights.begin(), engine->render.lights.end(), this));
	}
	//Export/Import lights
	Light::Light(YAML::Node node) : shaderLightID(engine->render.GetLightID(node["type"].as<std::string>())), Component(node) {
		diffuse = node["diffuse"].as<Utilities::Vector3>();
		specular = node["specular"].as<Utilities::Vector3>();
	}
	DirectionalLight::DirectionalLight(YAML::Node node) : Light(node) {
		engine->render.lights.push_back(this);
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

		engine->render.lights.push_back(this);
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

		engine->render.lights.push_back(this);
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
