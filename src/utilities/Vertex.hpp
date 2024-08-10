#pragma once
#include <cstddef>

namespace StevEngine {
    namespace Utilities {
        struct Vertex {
            float x, y, z;
            float texX, texY;
            Vertex(float x, float y, float z, float texX = 0, float texY = 0)
                : x(x), y(y), z(z), texX(texX), texY(texY) {}
            bool operator==(const Vertex o) const;
            static size_t size;
        };
    }
}
