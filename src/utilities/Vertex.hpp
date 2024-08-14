#pragma once
#include "utilities/Vector2.hpp"
#include "utilities/Vector3.hpp"
#include <cstddef>

namespace StevEngine {
    namespace Utilities {
        struct Vertex {
            float x, y, z;
            float u, v;
            float nX, nY, nZ;
            Vertex(float x, float y, float z, float nX, float nY, float nZ, float u = 0, float v = 0)
                : x(x), y(y), z(z), nX(nX), nY(nY), nZ(nZ), u(u), v(v) {}
            Vertex(Vector3 position, Vector3 normal, Vector2 texture = Vector2(0,0))
                    : x(position.X), y(position.Y), z(position.Z), nX(normal.X), nY(normal.Y), nZ(normal.Z), u(texture.X), v(texture.Y) {}
            Vertex() {};
            bool operator==(const Vertex o) const;
            static size_t size;
        };
    }
}
