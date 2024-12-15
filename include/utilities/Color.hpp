#pragma once
#include <stdint.h>

namespace StevEngine {
	namespace Utilities {
		struct Color {
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
			Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
			Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b), a(255) {}
			Color(uint8_t v) : r(v), g(v), b(v), a(255) {}
			Color() : r(255), g(255), b(255), a(255) {};
			const float* data() const;
		};
	}
}

#include <yaml-cpp/yaml.h>
namespace YAML {
	template<> struct convert<StevEngine::Utilities::Color> {
		static Node encode(const StevEngine::Utilities::Color& rhs);
		static bool decode(const Node& node, StevEngine::Utilities::Color& rhs);
	};
}
