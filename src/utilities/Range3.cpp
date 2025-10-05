#include "Range3.hpp"
#include "Vector3.hpp"
#include "utilities/Stream.hpp"
#include <format>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

namespace StevEngine::Utilities {
	//Constructors
	Range3::Range3() {
		Low = Vector3(0, 0, 0);
		High = Vector3(0, 0, 0);
	}
	Range3::Range3(double lowX, double highX, double lowY, double highY, double lowZ, double highZ) {
		Low = Vector3(lowX, lowY, lowZ);
		High = Vector3(highX, highY, highZ);
	}
	Range3::Range3(const Vector3& low, const Vector3& high) {
		Low = low;
		High = high;
	}
	//Functions
	Vector3 Range3::GetCenter() const {
		return Vector3(Low.X + ((High.X - Low.X) / 2), Low.Y + ((High.Y - Low.Y) / 2), Low.Z + ((High.Z - Low.Z) / 2));
	}

	Vector3 Range3::GetSize() const {
		return High - Low;
	}
	//Conversions
	Range3::operator std::string() const {
		return std::format("[({}), ({})]", (std::string)Low, (std::string)High);
	}
	#ifdef StevEngine_PHYSICS
	Range3::operator JPH::AABox() const {
		return JPH::AABox((JPH::DVec3)Low, (JPH::DVec3)High);
	}
	Range3& Range3::operator= (const JPH::AABox& other) {
		Low = other.mMin;
		High = other.mMax;
		return *this;
	}
	Range3::Range3(const JPH::AABox& other) {
		Low = other.mMin;
		High = other.mMax;
	}
	#endif

	//Read from stream
	template <> Utilities::Range3 Stream::Read<Utilities::Range3>() {
		Utilities::Range3 value;
		*this >> value.Low >> value.High;
		return value;
	}
	//Write to stream
	template <> void Stream::Write<Utilities::Range3>(const Utilities::Range3& data) {
		*this << data.Low << data.High;
	}
}
