#include <vector>
#include <SDL2/SDL.h>
#include <glm/mat4x4.hpp>

#include <utilities/Vector3.hpp>
#include <utilities/Quaternion.hpp>

namespace StevEngine {
    namespace Render {
        class System;
        
        class Object {
			friend class System;
            public:
                Object(std::vector<float> vertices, std::vector<unsigned int> indices, glm::mat4x4 transform, SDL_Color color = SDL_Color(1, 1, 1, 1));
            private:
                std::vector<float> vertices;
                std::vector<unsigned int> indices;
                SDL_Color color;
                glm::mat4x4 transform;
		};
    }
}