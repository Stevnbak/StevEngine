#pragma once
#include <string>

namespace StevEngine::Utilities {
	class Vector3;

	class Vector2 {
		public:
			//Values
			double X, Y;
			//Constructors
			Vector2(double x, double y);
			Vector2(const Vector2& from);
			Vector2(std::string str);
			Vector2();
			//Functions
			Vector2 Get() const;
			double Magnitude() const;
			Vector2& Normalize();
			Vector2 Normalized() const;
			Vector2& Mult(double value);
			Vector2& Divide(double value);
			//Operators
			Vector2  operator   +   (const Vector2& other) const;
			Vector2  operator   -   (const Vector2& other) const;
			Vector2& operator   +=  (const Vector2& other);
			Vector2& operator   -=  (const Vector2& other);
			Vector2  operator   *   (const double& other) const;
			Vector2  operator   /   (const double& other) const;
			bool	 operator   ==  (const Vector2& other) const;
			//Conversions
			explicit operator Vector3() const;
			explicit operator std::string() const;
			const float* data() const;
			//Static stuff
			static Vector2 up, right;
			static double Distance(const Vector2& a, const Vector2& b);
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
