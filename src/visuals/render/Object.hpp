#pragma once
#ifdef StevEngine_RENDERER_GL
#include <vector>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include <utilities/Vector3.hpp>
#include <utilities/Quaternion.hpp>
#include "utilities/Vertex.hpp"

namespace StevEngine {
    namespace Render {
        class System;

        class Object {
			friend class System;
            public:
                Object(std::vector<Utilities::Vertex> vertices, SDL_Color color = SDL_Color(1, 1, 1, 1), SDL_Surface* textureData = nullptr);
                Object(std::vector<Utilities::Vertex> vertices, std::vector<unsigned int> indices, SDL_Color color = SDL_Color(1, 1, 1, 1), SDL_Surface* textureData = nullptr);
                SDL_Color color;
            private:
                std::vector<float> vertices;
                std::vector<unsigned int> indices;
                GLuint texture;
                bool textured;
		};
    }
}
#endif
