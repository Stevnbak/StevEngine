#pragma once
#include <cstdlib>

namespace StevEngine::Utilities {
	/**
	 * @brief Get random double in range
	 * @param min Minimum value
	 * @param max Maximum value
	 * @return Random value between min and max
	 */
	double GetRandomDouble(double min, double max);

	/**
	 * @brief Get random double up to max
	 * @param max Maximum value
	 * @return Random value between 0 and max
	 */
	double GetRandomDouble(double max);

	/**
	 * @brief Get random integer in range
	 * @param min Minimum value (inclusive)
	 * @param max Maximum value (exclusive)
	 * @return Random integer between min and max
	 */
	int GetRandomInt(int min, int max);

	/**
	 * @brief Get random integer up to max
	 * @param max Maximum value (exclusive)
	 * @return Random integer between 0 and max
	 */
	int GetRandomInt(int max);
}
