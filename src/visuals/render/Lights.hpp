#pragma once
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
			protected:
				Light(unsigned int shaderID, Utilities::Vector3 diffuse, Utilities::Vector3 specular, std::string type);
				Light(YAML::Node node);
				const unsigned int shaderLightID;
				virtual void UpdateShader() = 0;
				virtual ~Light();
		};

		class DirectionalLight final : public Light {
			friend class StevEngine::Render::RenderSystem;
			public:
				DirectionalLight(Utilities::Vector3 diffuse = Utilities::Vector3(1.0), Utilities::Vector3 specular = Utilities::Vector3(1.0));
				DirectionalLight(YAML::Node node);
				YAML::Node Export(YAML::Node node) const;
			private:
				void UpdateShader();
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
			private:
				void UpdateShader();
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
			private:
				void UpdateShader();
				~SpotLight();
		};
		inline bool sl = CreateComponents::RegisterComponentType<SpotLight>("SpotLight");
	}
}
#endif
