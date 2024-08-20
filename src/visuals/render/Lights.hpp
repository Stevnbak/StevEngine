#pragma once
#ifdef StevEngine_RENDERER_GL
#include "scenes/Component.hpp"
#include "utilities/Vector3.hpp"

namespace StevEngine {
    namespace Render {
        class System;
        class Light : public Component {
            friend class StevEngine::GameObject;
            friend class StevEngine::Render::System;

            public:
                Utilities::Vector3 diffuse;
                Utilities::Vector3 specular;
			protected:
			    Light(unsigned int shaderID, Utilities::Vector3 diffuse, Utilities::Vector3 specular, std::string type);
				const unsigned int shaderLightID;
				virtual void Export(tinyxml2::XMLElement* element) = 0;
				virtual void UpdateShader() = 0;
				virtual ~Light();
			private:
                void Update(double deltaTime) {};
                void Draw(glm::mat4x4 transform) {};
                void Deactivate() {};
				void Start() {};
        };

        class DirectionalLight final : public Light {
            friend class StevEngine::Render::System;
            public:
                DirectionalLight(Utilities::Vector3 diffuse = Utilities::Vector3(1.0), Utilities::Vector3 specular = Utilities::Vector3(1.0));
            private:
                void Export(tinyxml2::XMLElement* element) {};
                void UpdateShader();
				~DirectionalLight();
        };

        class PointLight final : public Light {
            friend class StevEngine::Render::System;

            public:
                PointLight(Utilities::Vector3 diffuse = Utilities::Vector3(1.0), Utilities::Vector3 specular = Utilities::Vector3(1.0), float constant = 1.0, float linear = 0.02, float quadratic = 0.0017);

                float constant;
                float linear;
                float quadratic;
            private:
                void Export(tinyxml2::XMLElement* element) {};
                void UpdateShader();
				~PointLight();
        };

        class SpotLight final : public Light {
            friend class StevEngine::Render::System;

            public:
                SpotLight(Utilities::Vector3 diffuse = Utilities::Vector3(1.0), Utilities::Vector3 specular = Utilities::Vector3(1.0), float cutOff = 12.5, float outerCutOff = 17.5);

                float cutOff;
                float outerCutOff;
            private:
                void Export(tinyxml2::XMLElement* element) {};
                void UpdateShader();
                ~SpotLight();
        };
    }
}

#endif
