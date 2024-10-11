#pragma once
#include "utilities/Vector2.hpp"
#include "utilities/Vector3.hpp"
#include <cstddef>

namespace StevEngine {
	namespace Utilities {
		const int VERTEX_COUNT = (3 + 3 + 2);
		const size_t VERTEX_SIZE = (VERTEX_COUNT) * sizeof(float);
		struct Vertex {
			Vector3 position;
			Vector2 uv;
			Vector3 normal;
			Vertex(float x, float y, float z, float nX, float nY, float nZ, float u = 0, float v = 0)
				: position(x, y, z), normal(nX, nY, nZ), uv(u, v) {}
			Vertex(Vector3 position, Vector3 normal, Vector2 uv = Vector2(0,0))
					: position(position), normal(normal), uv(uv) {}
			Vertex() {};
			bool operator==(const Vertex o) const;
		};
	}
}
