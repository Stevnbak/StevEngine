#pragma once
#include <cstddef>
#ifdef StevEngine_RENDERER_GL
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <vector>
#include <type_traits>

#include "Object.hpp"

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
                void SetBackground(SDL_Color color);
                void SetAmbientLight(float strength, SDL_Color color = {255,255,255,255});
			private:
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
                //Shaders
                unsigned int shaderProgram;
                //GPU Buffers
                unsigned int VBO; //Vertex Buffer Object
                unsigned int EBO; //Element Buffer Object
                unsigned int VAO; //Vertex Array Object
                //Background
                SDL_Color backgroundColor = (SDL_Color){0, 0, 0, 255};
		};
    }
}
#endif
