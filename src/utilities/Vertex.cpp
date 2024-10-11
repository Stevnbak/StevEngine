#include "Vertex.hpp"
#include <array>

namespace StevEngine::Utilities {
	bool Vertex::operator== (const Vertex o) const {
		return o.position == position && o.normal == normal && o.uv == uv;
	}
}
