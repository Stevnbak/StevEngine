#pragma once
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include "Object.hpp"
#include "CustomObject.hpp"

namespace StevEngine {
    class Engine;
    namespace Render {
        class System {
			friend class StevEngine::Engine;
			public:
				void DrawObject(Object object, glm::mat4x4 transform);
				void DrawCustomObject(CustomObject* object, glm::mat4x4 transform);
			private:
                System();
                void Init();
                void StartFrame();
                void EndFrame();
                unsigned int shaderProgram;
                unsigned int VBO;
                unsigned int EBO;
                unsigned int VAO;
		};
    }
}