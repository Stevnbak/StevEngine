#pragma once
#include "visuals/shaders/ShaderProgram.hpp"
#ifdef StevEngine_RENDERER_GL
#include "scenes/Component.hpp"
#include "utilities/Vector3.hpp"

namespace StevEngine {
	namespace Render {
		class RenderSystem;
		class Light : public Component {
			friend class StevEngine::Render::RenderSystem;
			public:
				Utilities::Vector3 diffuse;
				Utilities::Vector3 specular;
				virtual void UpdateShader(const ShaderProgram& program) const = 0;
				virtual void ResetShader(const ShaderProgram& program) const = 0;
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

				void UpdateShader(const ShaderProgram& program) const;
				void ResetShader(const ShaderProgram& program) const;
				~DirectionalLight();
		};
		inline bool dl = CreateComponents::RegisterComponentType<DirectionalLight>("DirectionalLight");

		class PointLight final : public Light {
			friend class StevEngine::Render::RenderSystem;
			public:
				PointLight(Utilities::Vector3 diffuse = Utilities::Vector3(1.0), Utilities::Vector3 specular = Utilities::Vector3(1.0), float constant = 1.0, float linear = 0.02, float quadratic = 0.0017);
				PointLight(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;

				float constant;
				float linear;
				float quadratic;

				void UpdateShader(const ShaderProgram& program) const;
				void ResetShader(const ShaderProgram& program) const;
				~PointLight();
		};
		inline bool pl = CreateComponents::RegisterComponentType<PointLight>("PointLight");

		class SpotLight final : public Light {
			friend class StevEngine::Render::RenderSystem;

			public:
				SpotLight(Utilities::Vector3 diffuse = Utilities::Vector3(1.0), Utilities::Vector3 specular = Utilities::Vector3(1.0), float cutOff = 12.5, float outerCutOff = 17.5);
				SpotLight(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;

				float cutOff;
				float outerCutOff;

				void UpdateShader(const ShaderProgram& program) const;
				void ResetShader(const ShaderProgram& program) const;
				~SpotLight();
		};
		inline bool sl = CreateComponents::RegisterComponentType<SpotLight>("SpotLight");
	}
}
#endif
