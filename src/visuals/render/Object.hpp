#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include <utilities/Vector3.hpp>
#include <utilities/Quaternion.hpp>

namespace StevEngine {
    namespace Render {
        class System;
        
        struct Vertex {
            float x, y, z;
            float texX, texY;
            Vertex(float x, float y, float z, float texX = 0, float texY = 0) : x(x), y(y), z(z), texX(texX), texY(texY) {}
            bool operator==(const Vertex o) const;
            static GLsizei size;
        };

        class Object {
			friend class System;
            public:
                Object(std::vector<Vertex> vertices, SDL_Color color = SDL_Color(1, 1, 1, 1), SDL_Surface* textureData = nullptr);
                SDL_Color color;
            private:
                std::vector<float> vertices;
                std::vector<unsigned int> indices;
                GLuint texture;
                bool textured;
		};
    }
}