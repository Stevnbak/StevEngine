#pragma once
#ifdef StevEngine_RENDERER_GL
#include <vector>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include <utilities/Vector3.hpp>
#include <utilities/Quaternion.hpp>
#include "utilities/Vertex.hpp"
#include "utilities/Texture.hpp"

namespace StevEngine {
    namespace Render {
        class System;

        struct Material {
            Utilities::Vector3 ambient = Utilities::Vector3(1.0);
            Utilities::Vector3 diffuse = Utilities::Vector3(1.0);
            Utilities::Vector3 specular = Utilities::Vector3(1.0);
            float shininess = 1.0;
        };

        class Object {
			friend class System;
            public:
                Object(std::vector<Utilities::Vertex> vertices, SDL_Color color = {255, 255, 255, 255}, Utilities::Texture textureData = NULL);
                Object(std::vector<Utilities::Vertex> vertices, std::vector<unsigned int> indices, SDL_Color color = {255, 255, 255, 255}, Utilities::Texture textureData = NULL);
                void SetTexture(Utilities::Texture textureData);
                SDL_Color color;
                Material material;
            private:
                std::vector<float> vertices;
                std::vector<unsigned int> indices;
                GLuint texture;
                bool textured;
		};
    }
}
#endif
