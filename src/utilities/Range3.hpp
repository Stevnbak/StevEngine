#pragma once
#include "Vector3.hpp"

#ifdef StevEngine_PHYSICS
#include <Jolt/Jolt.h>
#include <Jolt/Geometry/AABox.h>
#endif

namespace StevEngine::Utilities {
	class Vector3;

	/**
	 * @brief Axis-aligned bounding box in 3D space
	 *
	 * Represents a box in 3D space defined by minimum and maximum points.
	 * Used for bounds checking and collision detection.
	 */
	class Range3 {
		public:
			//Values
			Vector3 Low;   ///< Minimum point (lower bounds)
			Vector3 High;  ///< Maximum point (upper bounds)

			//Constructors
			/** @brief Create empty range at origin */
			Range3();

			/**
			 * @brief Create range from component bounds
			 * @param lowX Minimum X
			 * @param highX Maximum X
			 * @param lowY Minimum Y
			 * @param highY Maximum Y
			 * @param lowZ Minimum Z
			 * @param highZ Maximum Z
			 */
			Range3(double lowX, double highX, double lowY, double highY, double lowZ, double highZ);

			/**
			 * @brief Create range from min/max vectors
			 * @param low Minimum point
			 * @param high Maximum point
			 */
			Range3(const Vector3& low, const Vector3& high);

			/**
			 * @brief Get center point of range
			 * @return Center position vector
			 */
			Vector3 GetCenter() const;

			/**
			 * @brief Get size of range
			 * @return Size vector
			 */
			Vector3 GetSize() const;

			//Conversions
			#ifdef StevEngine_PHYSICS
			operator JPH::AABox() const;					///< Convert to Jolt AABB
			Range3& operator= (const JPH::AABox& other);	///< Assign from Jolt AABB
			Range3(const JPH::AABox& other);			   ///< Create from Jolt AABB
			#endif
	};
}
