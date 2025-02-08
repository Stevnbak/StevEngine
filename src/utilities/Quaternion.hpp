#pragma once
#ifdef StevEngine_PHYSICS
#include <Jolt/Jolt.h>
#endif
#include <string>

#include "Vector3.hpp"

namespace StevEngine::Utilities {
	class Vector4;

	/**
	 * @brief Quaternion for 3D rotations
	 *
	 * Implements quaternion mathematics for representing and manipulating
	 * 3D rotations. Provides operations for combining rotations and
	 * converting between different rotation representations.
	 */
	class Quaternion {
		public:
			//Values
			double W, X, Y, Z;  ///< Quaternion components (w + xi + yj + zk)

			//Constructors
			/**
			 * @brief Create quaternion from components
			 * @param w Real component
			 * @param x i component
			 * @param y j component
			 * @param z k component
			 */
			Quaternion(double w, double x, double y, double z);

			/**
			 * @brief Create quaternion from real and vector parts
			 * @param w Real component
			 * @param vector Vector component (x,y,z)
			 */
			Quaternion(double w, Vector3 vector);

			/**
			 * @brief Copy constructor
			 * @param from Quaternion to copy
			 */
			Quaternion(const Quaternion& from);

			/** @brief Create identity quaternion */
			Quaternion();

			//Directions
			/**
			 * @brief Get forward direction
			 * @return Forward vector in local space
			 */
			Vector3 Forward() const;

			/**
			 * @brief Get right direction
			 * @return Right vector in local space
			 */
			Vector3 Right() const;

			/**
			 * @brief Get up direction
			 * @return Up vector in local space
			 */
			Vector3 Up() const;

			/**
			 * @brief Conjugate this quaternion
			 * @return Reference to this
			 */
			Quaternion& Conjugate();

			/**
			 * @brief Invert this quaternion
			 * @return Reference to this
			 */
			Quaternion& Inverse();

			//Functions
			Quaternion Get() const;			  ///< Get copy of this quaternion
			double Magnitude() const;			///< Get magnitude
			Quaternion& Normalize();			 ///< Normalize this quaternion
			Quaternion Normalized() const;	   ///< Get normalized copy

			/**
			 * @brief Get angle and axis representation
			 * @return Tuple of (angle, axis)
			 */
			std::tuple<double, Vector3> GetAngleAxis() const;

			//Operators
			Quaternion  operator+ (const Quaternion& other) const;  ///< Addition
			Quaternion  operator- (const Quaternion& other) const;  ///< Subtraction
			Quaternion  operator* (const Quaternion& other) const;  ///< Quaternion multiplication
			Quaternion  operator* (const double& other) const;	  ///< Scalar multiplication
			Quaternion  operator/ (const double& other) const;	  ///< Scalar division
			Quaternion& operator*= (const Quaternion& other);	   ///< Quaternion multiply-assign
			bool operator== (const Quaternion& other) const;		///< Equality comparison

			//Vector operators
			/**
			 * @brief Rotate vector by quaternion
			 * @param v Vector to rotate
			 * @return Rotated vector
			 */
			Vector3 operator*(const Vector3& v) const;

			//Conversions
			explicit operator std::string() const;  ///< Convert to string
			explicit operator Vector4() const;	  ///< Convert to Vector4

			#ifdef StevEngine_PHYSICS
			operator JPH::Quat() const;					///< Convert to Jolt quaternion
			Quaternion& operator= (const JPH::Quat& other);  ///< Assign from Jolt quaternion
			Quaternion(const JPH::Quat& other);			 ///< Create from Jolt quaternion
			#endif

			//Static methods
			/**
			 * @brief Create quaternion from angle and axis
			 * @param angle Rotation angle in radians
			 * @param axis Rotation axis (normalized)
			 * @return Created quaternion
			 */
			static Quaternion FromAngleAxis(double angle, Vector3 axis);

			/**
			 * @brief Get angle between quaternions
			 * @param a First quaternion
			 * @param b Second quaternion
			 * @return Angle in radians
			 */
			static double Angle(const Quaternion& a, const Quaternion& b);

			/**
			 * @brief Linear interpolation between quaternions
			 * @param a Start quaternion
			 * @param b End quaternion
			 * @param t Interpolation factor (0-1)
			 * @return Interpolated quaternion
			 */
			static Quaternion Lerp(const Quaternion& a, const Quaternion& b, double t);

			/**
			 * @brief Spherical interpolation between quaternions
			 * @param a Start quaternion
			 * @param b End quaternion
			 * @param t Interpolation factor (0-1)
			 * @return Interpolated quaternion
			 */
			static Quaternion Slerp(const Quaternion& a, const Quaternion& b, double t);

			/**
			 * @brief Create rotation from start to end direction
			 * @param fromDirection Start direction
			 * @param toDirection Target direction
			 * @return Rotation quaternion
			 */
			static Quaternion FromToRotation(Vector3 fromDirection, Vector3 toDirection);

			/**
			 * @brief Create rotation looking in direction
			 * @param forward Forward direction
			 * @param upwards Up direction
			 * @return Look rotation
			 */
			static Quaternion LookRotation(Vector3 forward, const Vector3& upwards = Vector3::up);

			/**
			 * @brief Create inverse of quaternion
			 * @param q Quaternion to invert
			 * @return Inverted quaternion
			 */
			static Quaternion Inverse(const Quaternion& q);

			/**
			 * @brief Create conjugate of quaternion
			 * @param q Quaternion to conjugate
			 * @return Conjugated quaternion
			 */
			static Quaternion Conjugate(const Quaternion& q);

			/**
			 * @brief Calculate dot product
			 * @param a First quaternion
			 * @param b Second quaternion
			 * @return Dot product
			 */
			static double Dot(const Quaternion& a, const Quaternion& b);

			/**
			 * @brief Convert degrees to radians
			 * @param degrees Angle in degrees
			 * @return Angle in radians
			 */
			static double DegreesToRadians(double degrees);

			/**
			 * @brief Convert radians to degrees
			 * @param radians Angle in radians
			 * @return Angle in degrees
			 */
			static double RadiansToDegrees(double radians);
	};
}


#include <yaml-cpp/yaml.h>
namespace YAML {
	template<> struct convert<StevEngine::Utilities::Quaternion> {
		static Node encode(const StevEngine::Utilities::Quaternion& rhs);
		static bool decode(const Node& node, StevEngine::Utilities::Quaternion& rhs);
	};
}
