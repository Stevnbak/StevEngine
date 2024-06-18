#pragma once
#include <Jolt/Jolt.h>
#include <string>

#include "Vector3.hpp"

namespace StevEngine {
	namespace Utilities {
        
        class Range3 {
			public:
				Vector3 Low;
				Vector3 High;
				Range3();
				Range3(double lowX, double highX, double lowY, double highY, double lowZ, double highZ);
				Range3(Vector3 low, Vector3 high);
				Vector3 GetCenter();
		};
    }
}