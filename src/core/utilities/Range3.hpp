#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Geometry/AABox.h>
#include <string>

namespace StevEngine {
	namespace Utilities {
		class Vector3;
        
        class Range3 {
			public:
				//Values
				Vector3 Low;
				Vector3 High;
				//Constructors
				Range3();
				Range3(double lowX, double highX, double lowY, double highY, double lowZ, double highZ);
				Range3(Vector3 low, Vector3 high);
				//Functions
				Vector3 GetCenter();
				//Conversions
				operator JPH::AABox();
				Range3& operator= (const JPH::AABox& other);
				Range3(const JPH::AABox& other);
		};
    }
}