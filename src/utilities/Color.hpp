#pragma once
#include <stdint.h>

namespace StevEngine {
	namespace Utilities {
		/**
		 * @brief RGBA color representation
		 *
		 * Stores color data with 8 bits per channel.
		 * Provides conversions between byte and float formats.
		 */
		struct Color {
			uint8_t r;  ///< Red channel (0-255)
			uint8_t g;  ///< Green channel (0-255)
			uint8_t b;  ///< Blue channel (0-255)
			uint8_t a;  ///< Alpha channel (0-255)

			/**
			 * @brief Create color with all channels
			 * @param r Red value
			 * @param g Green value
			 * @param b Blue value
			 * @param a Alpha value
			 */
			Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}

			/**
			 * @brief Create opaque color
			 * @param r Red value
			 * @param g Green value
			 * @param b Blue value
			 */
			Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b), a(255) {}

			/**
			 * @brief Create grayscale color
			 * @param v Gray value for RGB
			 */
			Color(uint8_t v) : r(v), g(v), b(v), a(255) {}

			/** @brief Create white color */
			Color() : r(255), g(255), b(255), a(255) {};

			/**
			 * @brief Get color as float array
			 * @return Array of normalized float values [r,g,b,a]
			 */
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
