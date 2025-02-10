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
}

namespace StevEngine {
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

namespace YAML {
	Node convert<StevEngine::Utilities::Color>::encode(const StevEngine::Utilities::Color& rhs) {
		Node node;
		node.push_back((uint32_t)rhs.r);
		node.push_back((uint32_t)rhs.g);
		node.push_back((uint32_t)rhs.b);
		node.push_back((uint32_t)rhs.a);
		return node;
	}
	bool convert<StevEngine::Utilities::Color>::decode(const Node& node, StevEngine::Utilities::Color& rhs) {
		if(!node.IsSequence() || node.size() != 4) return false;
		rhs.r = node[0].as<uint32_t>();
		rhs.g = node[1].as<uint32_t>();
		rhs.b = node[2].as<uint32_t>();
		rhs.a = node[3].as<uint32_t>();
		return true;
	}
}
