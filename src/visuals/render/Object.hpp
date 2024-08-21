#pragma once
#ifdef StevEngine_RENDERER_GL
#include <vector>
#include <SDL.h>
#include <glad/gl.h>
#include <glm/mat4x4.hpp>

#include "utilities/Vector3.hpp"
#include "utilities/Quaternion.hpp"
#include "utilities/Vertex.hpp"
#include "visuals/Texture.hpp"
#include "utilities/Color.hpp"

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
                Object(std::vector<Utilities::Vertex> vertices, Utilities::Color color = Utilities::Color(255, 255, 255, 255), Visuals::Texture textureData = Visuals::Texture::empty);
                Object(std::vector<Utilities::Vertex> vertices, std::vector<unsigned int> indices, Utilities::Color color = Utilities::Color(255, 255, 255, 255), Visuals::Texture textureData = Visuals::Texture::empty);
                void SetTexture(Visuals::Texture textureData);
                void FreeTexture();
                Utilities::Color color;
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
