#pragma once
#include "Vector3.hpp"

#ifdef StevEngine_PHYSICS
#include <Jolt/Jolt.h>
#include <Jolt/Geometry/AABox.h>
#endif
#include <string>

namespace StevEngine::Utilities {
	class Vector3;

	class Range3 {
		public:
			//Values
			Vector3 Low;
			Vector3 High;
			//Constructors
			Range3();
			Range3(double lowX, double highX, double lowY, double highY, double lowZ, double highZ);
			Range3(const Vector3& low, const Vector3& high);
			//Functions
			Vector3 GetCenter() const;
			//Conversions
			#ifdef StevEngine_PHYSICS
			operator JPH::AABox() const;
			Range3& operator= (const JPH::AABox& other);
			Range3(const JPH::AABox& other);
			#endif
	};
}

#include <yaml-cpp/yaml.h>
namespace YAML {
	template<> struct convert<StevEngine::Utilities::Range3> {
		static Node encode(const StevEngine::Utilities::Range3& rhs);
		static bool decode(const Node& node, StevEngine::Utilities::Range3& rhs);
	};
}
