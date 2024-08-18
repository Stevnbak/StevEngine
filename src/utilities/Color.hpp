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
			Color() : r(0), g(0), b(0), a(255) {};
		};
	}
}