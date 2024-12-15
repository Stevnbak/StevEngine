#pragma once
#include "utilities/Vector2.hpp"
#include "utilities/Vector3.hpp"
#include <cstddef>

namespace StevEngine::Utilities {
	const int VERTEX_COUNT = (3 + 2 + 3 + 3);
	const size_t VERTEX_SIZE = (VERTEX_COUNT) * sizeof(float);
	struct Vertex {
		Vector3 position;
		Vector2 uv;
		Vector3 normal, tangent;
		Vertex(Vector3 position, Vector2 uv, Vector3 normal, Vector3 tangent)
				: position(position), uv(uv), normal(normal), tangent(tangent) {}
		Vertex() {};
		bool operator==(const Vertex o) const;
	};
}
