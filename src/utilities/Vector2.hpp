#pragma once
#include <string>

namespace StevEngine::Utilities {
	class Vector3;

	/**
	 * @brief 2D vector class
	 *
	 * Represents a point or direction in 2D space.
	 * Provides common vector operations and utilities.
	 */
	class Vector2 {
		public:
			//Values
			double X, Y;  ///< Vector components

			//Constructors
			/**
			 * @brief Create vector from components
			 * @param x X component
			 * @param y Y component
			 */
			Vector2(double x, double y);

			/**
			 * @brief Copy constructor
			 * @param from Vector to copy
			 */
			Vector2(const Vector2& from);

			/**
			 * @brief Create vector from string
			 * @param str String representation
			 */
			Vector2(std::string str);

			/** @brief Create zero vector */
			Vector2();

			//Functions
			Vector2 Get() const;	   ///< Get copy of this vector
			double Magnitude() const;  ///< Get vector length
			Vector2& Normalize();	  ///< Normalize this vector
			Vector2 Normalized() const;  ///< Get normalized copy

			//Operators
			Vector2  operator+ (const Vector2& other) const;  ///< Addition
			Vector2  operator- (const Vector2& other) const;  ///< Subtraction
			Vector2& operator+= (const Vector2& other);	   ///< Add-assign
			Vector2& operator-= (const Vector2& other);	   ///< Subtract-assign
			Vector2  operator* (const double& other) const;   ///< Scalar multiplication
			Vector2  operator/ (const double& other) const;   ///< Scalar division
			bool	 operator== (const Vector2& other) const; ///< Equality comparison

			//Conversions
			explicit operator Vector3() const;		///< Convert to Vector3
			explicit operator std::string() const;	///< Convert to string
			const float* data() const;			   ///< Get raw float array

			//Static vectors
			static Vector2 up;	 ///< Up vector (0,1)
			static Vector2 right;  ///< Right vector (1,0)

			/**
			 * @brief Calculate distance between vectors
			 * @param a First vector
			 * @param b Second vector
			 * @return Distance between points
			 */
			static double Distance(const Vector2& a, const Vector2& b);

			/**
			 * @brief Calculate dot product
			 * @param a First vector
			 * @param b Second vector
			 * @return Dot product
			 */
			static double Dot(const Vector2& a, const Vector2& b);
	};
}
#include <yaml-cpp/yaml.h>
namespace YAML {
	template<> struct convert<StevEngine::Utilities::Vector2> {
		static Node encode(const StevEngine::Utilities::Vector2& rhs);
		static bool decode(const Node& node, StevEngine::Utilities::Vector2& rhs);
	};
}
