#include "Vertex.hpp"

namespace StevEngine::Utilities {
	bool Vertex::operator== (const Vertex o) const {
		return o.position == position && o.uv == uv && o.normal == normal && o.tangent == tangent;
	}
}
