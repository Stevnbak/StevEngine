#pragma once
#include <string>
#include <format>

namespace Log {
	/**
	 * @brief Log a normal/info message
	 * @param msg Message to log
	 * @param fromEngine Whether message is from engine (vs game code)
	 */
	void Normal(std::string msg, bool fromEngine = false);

	/**
	 * @brief Log a debug message (only in debug builds)
	 * @param msg Message to log
	 * @param fromEngine Whether message is from engine
	 */
	void Debug(std::string msg, bool fromEngine = false);

	/**
	 * @brief Log an error message
	 * @param msg Message to log
	 * @param fromEngine Whether message is from engine
	 */
	void Error(std::string msg, bool fromEngine = false);

	/**
	 * @brief Log a warning message
	 * @param msg Message to log
	 * @param fromEngine Whether message is from engine
	 */
	void Warning(std::string msg, bool fromEngine = false);

	/** @brief Whether to show engine log messages */
	extern bool engineLogEnabled;

	#ifdef StevEngine_PLAYER_DATA
	/**
	 * @brief Initialize logging system
	 * @param path Directory path for log files
	 */
	void StartLogging(std::string path);

	/** @brief Close logging system and flush files */
	void CloseLogging();
	#endif
}