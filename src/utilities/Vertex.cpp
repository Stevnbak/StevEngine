#include "Vertex.hpp"

namespace StevEngine::Utilities {
    size_t Vertex::size = (3 + 2) * sizeof(float);
    bool Vertex::operator== (const Vertex o) const {
        return
            (o.x == x) &&
            (o.y == y) &&
            (o.z == z) &&
            (o.texX == texX) &&
            (o.texY == texY);
    }
}