#include "utilities/Vector3.hpp"
#ifdef StevEngine_RENDERER_GL
#include "Lights.hpp"
#include "visuals/render/System.hpp"
#include "main/Engine.hpp"
#include "scenes/GameObject.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace StevEngine::Render {
    Light::Light(unsigned int shaderID, Utilities::Vector3 diffuse, Utilities::Vector3 specular, std::string type)
		: shaderLightID(shaderID), shaderProgram(Engine::Instance->render.shaderProgram), diffuse(diffuse), specular(specular), Component(type) {};
    //Create light components
    DirectionalLight::DirectionalLight(Utilities::Vector3 diffuse, Utilities::Vector3 specular)
        : Light(Engine::Instance->render.GetLightID("DirectionalLight"), diffuse, specular, "DirectionalLight") {
        Engine::Instance->render.lights.push_back(this);
    }
    PointLight::PointLight(Utilities::Vector3 diffuse, Utilities::Vector3 specular, float constant, float linear, float quadratic)
        : Light(Engine::Instance->render.GetLightID("PointLight"), diffuse, specular, "PointLight"), constant(constant), linear(linear), quadratic(quadratic) {
        Engine::Instance->render.lights.push_back(this);
    }
    SpotLight::SpotLight(Utilities::Vector3 diffuse, Utilities::Vector3 specular, float cutOff, float outerCutOff)
        : Light(Engine::Instance->render.GetLightID("SpotLight"), diffuse, specular, "SpotLight"), cutOff(cutOff), outerCutOff(outerCutOff) {
        Engine::Instance->render.lights.push_back(this);
    }
    //Update shader information functions
    void DirectionalLight::UpdateShader() {
        std::string part = "directionalLights[" + std::to_string(shaderLightID) + "].";
        glUniform3fv(glGetUniformLocation(shaderProgram, (part + "basic.diffuse").c_str()), 1, glm::value_ptr((glm::vec3)diffuse));
        glUniform3fv(glGetUniformLocation(shaderProgram, (part + "basic.specular").c_str()), 1, glm::value_ptr((glm::vec3)specular));

        glUniform3fv(glGetUniformLocation(shaderProgram, (part + "direction").c_str()), 1, glm::value_ptr((glm::vec3)GetParent()->GetWorldRotation().Forward()));
    }
    DirectionalLight::~DirectionalLight() {
        std::string part = "directionalLights[" + std::to_string(shaderLightID) + "].";
        glUniform3fv(glGetUniformLocation(shaderProgram, (part + "basic.diffuse").c_str()), 1, glm::value_ptr(glm::vec3()));
        glUniform3fv(glGetUniformLocation(shaderProgram, (part + "basic.specular").c_str()), 1, glm::value_ptr(glm::vec3()));

        glUniform3fv(glGetUniformLocation(shaderProgram, (part + "direction").c_str()), 1, glm::value_ptr(glm::vec3()));
    }
    void PointLight::UpdateShader() {
        std::string part = "pointLights[" + std::to_string(shaderLightID) + "].";
        glUniform3fv(glGetUniformLocation(shaderProgram, (part + "basic.diffuse").c_str()), 1, glm::value_ptr((glm::vec3)diffuse));
        glUniform3fv(glGetUniformLocation(shaderProgram, (part + "basic.specular").c_str()), 1, glm::value_ptr((glm::vec3)specular));

        glUniform3fv(glGetUniformLocation(shaderProgram, (part + "position").c_str()), 1, glm::value_ptr((glm::vec3)GetParent()->GetWorldPosition()));

        glUniform1f (glGetUniformLocation(shaderProgram, (part + "constant").c_str()), constant);
        glUniform1f (glGetUniformLocation(shaderProgram, (part + "linear").c_str()), linear);
        glUniform1f (glGetUniformLocation(shaderProgram, (part + "quadratic").c_str()), quadratic);
    }
    PointLight::~PointLight() {
        std::string part = "pointLights[" + std::to_string(shaderLightID) + "].";
        glUniform3fv(glGetUniformLocation(shaderProgram, (part + "basic.diffuse").c_str()), 1, glm::value_ptr(glm::vec3()));
        glUniform3fv(glGetUniformLocation(shaderProgram, (part + "basic.specular").c_str()), 1, glm::value_ptr(glm::vec3()));

        glUniform3fv(glGetUniformLocation(shaderProgram, (part + "position").c_str()), 1, glm::value_ptr(glm::vec3()));

        glUniform1f (glGetUniformLocation(shaderProgram, (part + "constant").c_str()), 0);
        glUniform1f (glGetUniformLocation(shaderProgram, (part + "linear").c_str()), 0);
        glUniform1f (glGetUniformLocation(shaderProgram, (part + "quadratic").c_str()), 0);
    }
    void SpotLight::UpdateShader() {
        std::string part = "spotLights[" + std::to_string(shaderLightID) + "].";
        glUniform3fv(glGetUniformLocation(shaderProgram, (part + "basic.diffuse").c_str()), 1, glm::value_ptr((glm::vec3)diffuse));
        glUniform3fv(glGetUniformLocation(shaderProgram, (part + "basic.specular").c_str()), 1, glm::value_ptr((glm::vec3)specular));

        glUniform3fv(glGetUniformLocation(shaderProgram, (part + "position").c_str()), 1, glm::value_ptr((glm::vec3)GetParent()->GetWorldPosition()));
        glUniform3fv(glGetUniformLocation(shaderProgram, (part + "position").c_str()), 1, glm::value_ptr((glm::vec3)GetParent()->GetWorldRotation().Forward()));

        glUniform1f (glGetUniformLocation(shaderProgram, (part + "cutOff").c_str()), cutOff);
        glUniform1f (glGetUniformLocation(shaderProgram, (part + "outerCutOff").c_str()), outerCutOff);
    }
    SpotLight::~SpotLight() {
        std::string part = "spotLights[" + std::to_string(shaderLightID) + "].";
        glUniform3fv(glGetUniformLocation(shaderProgram, (part + "basic.diffuse").c_str()), 1, glm::value_ptr(glm::vec3()));
        glUniform3fv(glGetUniformLocation(shaderProgram, (part + "basic.specular").c_str()), 1, glm::value_ptr(glm::vec3()));

        glUniform3fv(glGetUniformLocation(shaderProgram, (part + "position").c_str()), 1, glm::value_ptr(glm::vec3()));
        glUniform3fv(glGetUniformLocation(shaderProgram, (part + "direction").c_str()), 1, glm::value_ptr(glm::vec3()));

        glUniform1f (glGetUniformLocation(shaderProgram, (part + "cutOff").c_str()), 0);
        glUniform1f (glGetUniformLocation(shaderProgram, (part + "outerCutOff").c_str()), 0);
    }
    Light::~Light() {
        Engine::Instance->render.lights.erase(std::find(Engine::Instance->render.lights.begin(), Engine::Instance->render.lights.end(), this));
    }
}
#endif
