#pragma once
#ifdef StevEngine_PHYSICS
#include <Jolt/Jolt.h>
#endif
#ifdef StevEngine_MODELS
#include "assimp/vector3.h"
#endif
#include <string>

namespace StevEngine::Utilities {
	class Vector2;

	/**
	 * @brief 3D vector class
	 *
	 * Represents a point or direction in 3D space.
	 * Provides common vector operations and utilities.
	 */
	class Vector3 {
		public:
			//Values
			double X, Y, Z;  ///< Vector components

			//Constructors
			/**
			 * @brief Create vector from components
			 * @param x X component
			 * @param y Y component
			 * @param z Z component
			 */
			Vector3(double x, double y, double z);

			/**
			 * @brief Copy constructor
			 * @param from Vector to copy
			 */
			Vector3(const Vector3& from);

			/**
			 * @brief Create vector with all components equal
			 * @param v Value for all components
			 */
			Vector3(double v);

			/** @brief Create zero vector */
			Vector3();

			//Functions
			Vector3 Get() const;	   ///< Get copy of this vector
			double Magnitude() const;  ///< Get vector length
			Vector3& Normalize();	  ///< Normalize this vector
			Vector3 Normalized() const; ///< Get normalized copy

			//Operators
			Vector3  operator+ (const Vector3& other) const;  ///< Addition
			Vector3  operator- (const Vector3& other) const;  ///< Subtraction
			Vector3  operator- () const;					  ///< Negation
			Vector3& operator+= (const Vector3& other);	   ///< Add-assign
			Vector3& operator-= (const Vector3& other);	   ///< Subtract-assign
			Vector3  operator* (const double& other) const;   ///< Scalar multiplication
			Vector3  operator/ (const double& other) const;   ///< Scalar division
			bool	 operator== (const Vector3& other) const; ///< Equality comparison

			//Conversions
			explicit operator Vector2() const;		///< Convert to Vector2
			explicit operator std::string() const;	///< Convert to string
			const float* data() const;			   ///< Get raw float array

			#ifdef StevEngine_PHYSICS
			operator JPH::DVec3() const;					///< Convert to Jolt double vector
			operator JPH::Vec3() const;					 ///< Convert to Jolt float vector
			Vector3& operator= (const JPH::Vec3& other);	///< Assign from Jolt float vector
			Vector3& operator= (const JPH::DVec3& other);   ///< Assign from Jolt double vector
			Vector3(const JPH::Vec3& other);			   ///< Create from Jolt float vector
			Vector3(const JPH::DVec3& other);			  ///< Create from Jolt double vector
			#endif

			#ifdef StevEngine_MODELS
			operator aiVector3D() const;					///< Convert to Assimp vector
			Vector3& operator= (const aiVector3D& other);   ///< Assign from Assimp vector
			Vector3(const aiVector3D& other);			  ///< Create from Assimp vector
			#endif

			//Static vectors
			static const Vector3 up;	   ///< Up vector (0,1,0)
			static const Vector3 right;	///< Right vector (1,0,0)
			static const Vector3 forward;  ///< Forward vector (0,0,-1)
			static const Vector3 identity; ///< One vector (1,1,1)
			static const Vector3 zero;	 ///< Zero vector (0,0,0)

			/**
			 * @brief Calculate distance between vectors
			 * @param a First vector
			 * @param b Second vector
			 * @return Distance between points
			 */
			static double Distance(const Vector3& a, const Vector3& b);

			/**
			 * @brief Calculate cross product
			 * @param a First vector
			 * @param b Second vector
			 * @return Cross product vector
			 */
			static Vector3 Cross(const Vector3& a, const Vector3& b);

			/**
			 * @brief Calculate dot product
			 * @param a First vector
			 * @param b Second vector
			 * @return Dot product
			 */
			static double Dot(const Vector3& a, const Vector3& b);

			/**
			 * @brief Combine scale vectors by multiplication
			 * @param a First scale
			 * @param b Second scale
			 * @return Combined scale
			 */
			static Vector3 CombineScale(const Vector3& a, const Vector3& b);

			/**
			 * @brief Spherical interpolation between vectors
			 * @param a Start vector
			 * @param b End vector
			 * @param t Interpolation factor (0-1)
			 * @return Interpolated vector
			 */
			static Vector3 Slerp(const Vector3& a, const Vector3& b, double t);
	};
}
