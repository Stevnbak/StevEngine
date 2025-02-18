#include "Color.hpp"
#include "utilities/Stream.hpp"

namespace StevEngine::Utilities {
	const float* Color::data() const {
		float* data = new float[4];
		data[0] = r / 255.0f;
		data[1] = g / 255.0f;
		data[2] = b / 255.0f;
		data[3] = a / 255.0f;
		return data;
	}

	//Read from stream
	template <> Utilities::Color Stream::Read<Utilities::Color>() {
		Utilities::Color value;
		*this >> value.r >> value.g >> value.b >> value.a;;
		return value;
	}
	//Write to stream
	template <> void Stream::Write<Utilities::Color>(const Utilities::Color& data) {
		*this << data.r << data.g << data.b << data.a;
	}
}
