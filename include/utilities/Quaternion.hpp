#pragma once
#ifdef StevEngine_PHYSICS
#include <Jolt/Jolt.h>
#endif
#include <string>

#include "Vector3.hpp"

namespace StevEngine::Utilities {
	class Vector4;
	class Quaternion {
		public:
			//Values
			double W, X, Y, Z;
			//Constructors
			Quaternion(double w, double x, double y, double z);
			Quaternion(double w, Vector3 vector);
			Quaternion(const Quaternion& from);
			Quaternion();
			//Directions
			Vector3 Forward() const;
			Vector3 Right() const;
			Vector3 Up() const;
			//Functions
			Quaternion Get() const;
			double Magnitude() const;
			Quaternion& Normalize();
			Quaternion Normalized() const;
			std::tuple<double, Vector3> GetAngleAxis() const;
			Quaternion& Conjugate();
			Quaternion& Inverse();
			//Operators
			Quaternion  operator	+   (const Quaternion& other) const;
			Quaternion  operator	-   (const Quaternion& other) const;
			Quaternion  operator	*   (const Quaternion& other) const;
			Quaternion  operator	*   (const double& other) const;
			Quaternion  operator	/   (const double& other) const;
			Quaternion& operator	*=  (const Quaternion& other);
			bool		operator	==  (const Quaternion& other) const;
			//Vector operators
			Vector3 operator*(const Vector3& other) const;
			//Conversions
			explicit operator std::string() const;
			explicit operator Vector4() const;
			#ifdef StevEngine_PHYSICS
			operator JPH::Quat() const;
			Quaternion& operator= (const JPH::Quat& other);
			Quaternion(const JPH::Quat& other);
			#endif
			//Static methods
			static Quaternion FromAngleAxis(double angle, Vector3 axis);
			static double Angle(const Quaternion& a, const Quaternion& b);
			static Quaternion Lerp(const Quaternion& a, const Quaternion& b, double t);
			static Quaternion Slerp(const Quaternion& a, const Quaternion& b, double t);
			static Quaternion FromToRotation(Vector3 fromDirection, Vector3 toDirection);
			static Quaternion LookRotation(Vector3 forward, const Vector3& upwards = Vector3::up);
			static Quaternion Conjugate(const Quaternion& q);
			static Quaternion Inverse(const Quaternion& q);
			static double Dot(const Quaternion& a, const Quaternion& b);
			static double DegreesToRadians(double degrees);
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
