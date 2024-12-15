#pragma once
#include "utilities/Vector2.hpp"
#include "utilities/Vector3.hpp"
#include <cstddef>

namespace StevEngine::Utilities {
	/** @brief Number of floats per vertex */
	const int VERTEX_COUNT = (3 + 2 + 3 + 3);

	/** @brief Total size of vertex in bytes */
	const size_t VERTEX_SIZE = (VERTEX_COUNT) * sizeof(float);

	/**
	 * @brief 3D mesh vertex data structure
	 *
	 * Contains position, texture coordinates, normal, and tangent data
	 * for a single vertex in a 3D mesh.
	 */
	struct Vertex {
		Vector3 position;  ///< Position in 3D space
		Vector2 uv;	   ///< Texture coordinates
		Vector3 normal;   ///< Surface normal
		Vector3 tangent;  ///< Tangent vector for normal mapping

		/**
		 * @brief Create vertex with all attributes
		 * @param position Vertex position
		 * @param uv Texture coordinates
		 * @param normal Surface normal
		 * @param tangent Tangent vector
		 */
		Vertex(Vector3 position, Vector2 uv, Vector3 normal, Vector3 tangent)
				: position(position), uv(uv), normal(normal), tangent(tangent) {}

		/** @brief Create empty vertex */
		Vertex() {};

		/**
		 * @brief Compare vertices for equality
		 * @param o Other vertex
		 * @return true if all attributes match
		 */
		bool operator==(const Vertex o) const;
	};
}
