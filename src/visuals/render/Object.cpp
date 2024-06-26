#include "Object.hpp"

namespace StevEngine {
    namespace Render {
        Object::Object(std::vector<float> vertices, std::vector<unsigned int> indices, glm::mat4x4 transform, SDL_Color color)
            : vertices(vertices), indices(indices), transform(transform), color(color) {}
    }
}