#pragma once
#ifdef StevEngine_PHYSICS
#include <Jolt/Jolt.h>
#endif
#include "glm/vec3.hpp"
#ifdef StevEngine_MODELS
#include "assimp/mesh.h"
#endif
#include <string>

namespace StevEngine::Utilities {
	class Vector2;

	class Vector3 {
		public:
			//Values
			double X, Y, Z;
			//Constructors
			Vector3(double x, double y, double z);
			Vector3(const Vector3& from);
			Vector3(double v);
			Vector3();
			//Functions
			Vector3 Get() const;
			double Magnitude() const;
			Vector3& Normalize();
			Vector3 Normalized() const;
			//Operators
			Vector3  operator   +   (const Vector3& other) const;
			Vector3  operator   -   (const Vector3& other) const;
			Vector3  operator   -   () const;
			Vector3& operator   +=  (const Vector3& other);
			Vector3& operator   -=  (const Vector3& other);
			Vector3  operator   *   (const double& other) const;
			Vector3  operator   /   (const double& other) const;
			bool	 operator   ==  (const Vector3& other) const;
			//Conversions
			explicit operator Vector2() const;
			explicit operator std::string() const;
			explicit operator glm::vec3() const;
			const float* data() const;
			#ifdef StevEngine_PHYSICS
			operator JPH::DVec3() const;
			operator JPH::Vec3() const;
			Vector3& operator= (const JPH::Vec3& other);
			Vector3& operator= (const JPH::DVec3& other);
			Vector3(const JPH::Vec3& other);
			Vector3(const JPH::DVec3& other);
			#endif
			#ifdef StevEngine_MODELS
			operator aiVector3D() const;
			Vector3& operator= (const aiVector3D& other);
			Vector3(const aiVector3D& other);
			#endif
			//Static stuff
			static const Vector3 up , right, forward, identity, zero;
			static double Distance(const Vector3& a, const Vector3& b);
			static Vector3 Cross(const Vector3& a, const Vector3& b);
			static double Dot(const Vector3& a, const Vector3& b);
			static Vector3 CombineScale(const Vector3& a, const Vector3& b);
			static Vector3 Slerp(const Vector3& a, const Vector3& b, double t);
	};
}

#include <yaml-cpp/yaml.h>
namespace YAML {
	template<> struct convert<StevEngine::Utilities::Vector3> {
		static Node encode(const StevEngine::Utilities::Vector3& rhs);
		static bool decode(const Node& node, StevEngine::Utilities::Vector3& rhs);
	};
}
