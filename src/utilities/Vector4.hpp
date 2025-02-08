#pragma once
#include "utilities/Vector3.hpp"
#include <string>

namespace StevEngine::Utilities {
	class Quaternion;

	/**
	 * @brief 4D vector class
	 *
	 * Represents a point or direction in 4D space.
	 * Used for homogeneous coordinates and quaternion math.
	 */
	class Vector4 {
		public:
			//Values
			double W, X, Y, Z;  ///< Vector components

			//Constructors
			/**
			 * @brief Create vector from components
			 * @param w W component
			 * @param x X component
			 * @param y Y component
			 * @param z Z component
			 */
			Vector4(double w, double x, double y, double z);

			/**
			 * @brief Copy constructor
			 * @param from Vector to copy
			 */
			Vector4(const Vector4& from);

			/**
			 * @brief Create from scalar and 3D vector
			 * @param w W component
			 * @param vec3 XYZ components
			 */
			Vector4(double w, const Vector3& vec3);

			/**
			 * @brief Create vector with all components equal
			 * @param v Value for all components
			 */
			Vector4(double v);

			/** @brief Create zero vector */
			Vector4();

			//Functions
			Vector4 Get() const;	   ///< Get copy of this vector
			double Magnitude() const;  ///< Get vector length
			Vector4& Normalize();	  ///< Normalize this vector
			Vector4 Normalized() const; ///< Get normalized copy

			/**
			 * @brief Calculate dot product
			 * @param a First vector
			 * @param b Second vector
			 * @return Dot product
			 */
			static double Dot(const Vector4& a, const Vector4& b);

			//Operators
			Vector4  operator+ (const Vector4& other) const;  ///< Addition
			Vector4  operator- (const Vector4& other) const;  ///< Subtraction
			Vector4  operator- () const;					  ///< Negation
			Vector4& operator+= (const Vector4& other);	   ///< Add-assign
			Vector4& operator-= (const Vector4& other);	   ///< Subtract-assign
			Vector4  operator* (const double& other) const;   ///< Scalar multiplication
			Vector4  operator/ (const double& other) const;   ///< Scalar division
			bool	 operator== (const Vector4& other) const; ///< Equality comparison

			//Conversions
			explicit operator std::string() const;	///< Convert to string
			explicit operator Quaternion() const;	 ///< Convert to Quaternion
			const float* data() const;			   ///< Get raw float array
	};
}

#include <yaml-cpp/yaml.h>
namespace YAML {
	template<> struct convert<StevEngine::Utilities::Vector4> {
		static Node encode(const StevEngine::Utilities::Vector4& rhs);
		static bool decode(const Node& node, StevEngine::Utilities::Vector4& rhs);
	};
}
