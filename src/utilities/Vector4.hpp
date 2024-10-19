#pragma once
#include "utilities/Vector3.hpp"
#include <string>

namespace StevEngine::Utilities {
	class Quaternion;
	class Vector4 {
		public:
			//Values
			double W, X, Y, Z;
			//Constructors
			Vector4(double w, double x, double y, double z);
			Vector4(const Vector4& from);
			Vector4(double w, const Vector3& vec3);
			Vector4(double v);
			Vector4();
			//Functions
			Vector4 Get() const;
			double Magnitude() const;
			Vector4& Normalize();
			Vector4 Normalized() const;
			static double Dot(const Vector4& a, const Vector4& b);
			//Operators
			Vector4  operator   +   (const Vector4& other) const;
			Vector4  operator   -   (const Vector4& other) const;
			Vector4  operator   -   () const;
			Vector4& operator   +=  (const Vector4& other);
			Vector4& operator   -=  (const Vector4& other);
			Vector4  operator   *   (const double& other) const;
			Vector4  operator   /   (const double& other) const;
			bool	 operator   ==  (const Vector4& other) const;
			//Conversions
			explicit operator std::string() const;
			explicit operator Quaternion() const;
			const float* data() const;
	};
}

#include <yaml-cpp/yaml.h>
namespace YAML {
	template<> struct convert<StevEngine::Utilities::Vector4> {
		static Node encode(const StevEngine::Utilities::Vector4& rhs);
		static bool decode(const Node& node, StevEngine::Utilities::Vector4& rhs);
	};
}
