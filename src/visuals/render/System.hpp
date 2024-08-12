#pragma once
#ifdef StevEngine_RENDERER_GL
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <vector>
#include <type_traits>

#include "Object.hpp"
#include "CustomObject.hpp"

namespace StevEngine {
    class Engine;
    namespace Render {
        class Light;
        class DirectionalLight;
        class PointLight;
        class SpotLight;
        class System {
			friend class StevEngine::Engine;
			friend class StevEngine::Render::Light;
			friend class StevEngine::Render::DirectionalLight;
			friend class StevEngine::Render::PointLight;
			friend class StevEngine::Render::SpotLight;

			public:
				void DrawObject(Object object, glm::mat4x4 transform);
				void DrawCustomObject(CustomObject* object, glm::mat4x4 transform);
                void SetBackground(SDL_Color color);
                void SetAmbientLight(float strength, SDL_Color color = {255,255,255,255});
			private:
                System();
                void Init();
                void StartFrame();
                void EndFrame();
                unsigned int shaderProgram;
                unsigned int VBO;
                unsigned int EBO;
                unsigned int VAO;
                SDL_Color backgroundColor = (SDL_Color){0, 0, 0};
                //Lights
                std::vector<Light*> lights;
                unsigned int GetLightID(std::string type);
		};
    }
}
#endif
