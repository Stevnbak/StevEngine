#include "Range3.hpp"

#include <core/Log.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

namespace StevEngine {
	namespace Utilities {
        Range3::Range3() {
			Low = Vector3(0, 0, 0);
			High = Vector3(0, 0, 0);
		}
		Range3::Range3(double lowX, double highX, double lowY, double highY, double lowZ, double highZ) {
			Low = Vector3(lowX, lowY, lowZ);
			High = Vector3(highX, highY, highZ);
		}
		Range3::Range3(Vector3 low, Vector3 high) {
			Low = low;
			High = high;
		}
		Vector3 Range3::GetCenter() {
			return Vector3(Low.X + ((High.X - Low.X) / 2), Low.Y + ((High.Y - Low.Y) / 2), Low.Z + ((High.Z - Low.Z) / 2));
		}
    }
}