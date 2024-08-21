#pragma once
#include "visuals/shaders/Shader.hpp"
#include "visuals/shaders/ShaderProgram.hpp"
#ifdef StevEngine_RENDERER_GL
#include <glm/mat4x4.hpp>
#include <vector>
#include <array>
#include <type_traits>
#include <cstddef>

#include "Object.hpp"
#include "utilities/Color.hpp"

namespace StevEngine {
    class Engine;
    namespace Render {
        //Lights
        class Light;
        class DirectionalLight;
        class PointLight;
        class SpotLight;

        //Object structs
        struct RenderObject {
            Object object;
            glm::mat4x4 transform;
        };

        //Render queues
        enum RenderQueue {
            STANDARD,
            TRANSPARENT,
            OVERLAY,

            MUST_BE_LAST
        };

        //System
        class System {
			friend class StevEngine::Engine;
			friend class StevEngine::Render::Light;
			friend class StevEngine::Render::DirectionalLight;
			friend class StevEngine::Render::PointLight;
			friend class StevEngine::Render::SpotLight;

			public:
				void DrawObject(Object object, glm::mat4x4 transform, RenderQueue queue = STANDARD);
                void SetBackground(Utilities::Color color);
                void SetAmbientLight(float strength, Utilities::Color color = Utilities::Color(255,255,255,255));
                void AddShader(ShaderProgram shader);
                void ResetShader(ShaderType type);
            protected:
			    //From Engine
                System();
                void Init();
                void DrawFrame();
                //From Lights
                std::vector<Light*> lights;
                unsigned int GetLightID(std::string type);
            private:
                //Queues
                std::array<std::vector<RenderObject>, RenderQueue::MUST_BE_LAST> queues;
                void Draw(RenderObject object);
                //Shader program
                ShaderProgram vertexShaderProgram;
                ShaderProgram fragmentShaderProgram;
                unsigned int shaderPipeline;
                //GPU Buffers
                unsigned int VBO; //Vertex Buffer Object
                unsigned int EBO; //Element Buffer Object
                unsigned int VAO; //Vertex Array Object
                //Background
                Utilities::Color backgroundColor = {0, 0, 0, 255};
		};
    }
}
#endif
