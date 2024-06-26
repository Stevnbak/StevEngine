#pragma once
#include <SDL2/SDL.h>
#include <glad/glad.h> 
#include <glm/mat4x4.hpp>

#include "Object.hpp"

namespace StevEngine {
    class Engine;
    namespace Render {
        class System {
			friend class StevEngine::Engine;
			public:
				void DrawObject(Object object);
			private:
                System();
                void Init();
                void StartFrame();
                void EndFrame();
                std::vector<Object> objects;
                unsigned int shaderProgram;
                unsigned int VBO;
                unsigned int EBO;
                unsigned int VAO;
		};
    }
}