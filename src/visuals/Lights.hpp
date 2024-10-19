#pragma once
#ifdef StevEngine_RENDERER_GL
#include "main/Component.hpp"
#include "utilities/Vector3.hpp"
#include "visuals/shaders/ShaderProgram.hpp"

namespace StevEngine::Visuals {
	class Light : public Component {
		public:
			Utilities::Vector3 diffuse;
			Utilities::Vector3 specular;
			virtual void UpdateShader(const Renderer::ShaderProgram& program) const = 0;
			virtual void ResetShader(const Renderer::ShaderProgram& program) const = 0;
			uint32_t GetShaderLightID() const { return shaderLightID; }
		protected:
			Light(uint32_t shaderID, Utilities::Vector3 diffuse, Utilities::Vector3 specular, std::string type);
			Light(YAML::Node node);
			const uint32_t shaderLightID;
			virtual ~Light();
	};

	class DirectionalLight final : public Light {
		public:
			DirectionalLight(Utilities::Vector3 diffuse = Utilities::Vector3(1.0), Utilities::Vector3 specular = Utilities::Vector3(1.0));
			DirectionalLight(YAML::Node node);
			YAML::Node Export(YAML::Node node) const;

			void UpdateShader(const Renderer::ShaderProgram& program) const;
			void ResetShader(const Renderer::ShaderProgram& program) const;
			~DirectionalLight();
	};
	inline bool dl = CreateComponents::RegisterComponentType<DirectionalLight>("DirectionalLight");

	class PointLight final : public Light {
		public:
			PointLight(Utilities::Vector3 diffuse = Utilities::Vector3(1.0), Utilities::Vector3 specular = Utilities::Vector3(1.0), float constant = 1.0, float linear = 0.02, float quadratic = 0.0017);
			PointLight(YAML::Node node);
			YAML::Node Export(YAML::Node node) const;

			float constant;
			float linear;
			float quadratic;

			void UpdateShader(const Renderer::ShaderProgram& program) const;
			void ResetShader(const Renderer::ShaderProgram& program) const;
			~PointLight();
	};
	inline bool pl = CreateComponents::RegisterComponentType<PointLight>("PointLight");

	class SpotLight final : public Light {
		public:
			SpotLight(Utilities::Vector3 diffuse = Utilities::Vector3(1.0), Utilities::Vector3 specular = Utilities::Vector3(1.0), float cutOff = 12.5, float outerCutOff = 17.5);
			SpotLight(YAML::Node node);
			YAML::Node Export(YAML::Node node) const;

			float cutOff;
			float outerCutOff;

			void UpdateShader(const Renderer::ShaderProgram& program) const;
			void ResetShader(const Renderer::ShaderProgram& program) const;
			~SpotLight();
	};
	inline bool sl = CreateComponents::RegisterComponentType<SpotLight>("SpotLight");
}
#endif
