#pragma once
#ifdef StevEngine_PLAYER_DATA
#include "main/Log.hpp"
#include "utilities/KeyValueStore.hpp"
#include <string>

namespace StevEngine::Data {

	/**
	 * @brief Manages persistent game data storage and retrieval
	 *
	 * GameData handles saving and loading persistent game data.
	 * It manages the application data directory and provides methods to read and
	 * write different data types.
	 */
	class GameData {
		public:
			/**
			 * @brief Initialize the data manager
			 * @param title Game title used for data directory name
			 *
			 * Creates necessary data directories and loads existing data if present.
			 */
			void Init(std::string title);

			/**
			 * @brief Read data of specified type
			 * @tparam T Type of data to read
			 * @param name Key name of the data
			 * @return Value of type T
			 *
			 * Reads and returns data of the specified type. Throws error if key
			 * doesn't exist.
			 */
			template<typename T> T Read(std::string name) {
				//Check if key exists
				if(!data.Contains(name)) {
					Log::Error(std::format("Game data with key '{}' not found!", name), true);
					return T();
				}
				//Read value
				return data.Read<T>(name);
			}

			/**
			 * @brief Save data of specified type
			 * @tparam T Type of data to save
			 * @param name Key name to save under
			 * @param data Data value to save
			 *
			 * Saves data and writes changes to disk immediately.
			 */
			template<typename T> void Save(std::string name, T data) {
				this->data.Write(name, data);
				SaveToFile();
			}

			/**
			 * @brief Delete saved data
			 * @param name Key name to delete
			 */
			void Delete(std::string name);

			/**
			 * @brief Get path to app data directory
			 * @return Path to app data directory
			 */
			std::string GetAppdataPath() { return appdataPath; }

			/**
			 * @brief Get path to log directory
			 * @return Path to log directory
			 */
			std::string GetLogPath() { return logPath; }

		private:
			/** @brief Path to app data directory */
			std::string appdataPath;

			/** @brief Path to log directory */
			std::string logPath;

			/** @brief Map containing all saved data streams */
			Utilities::KeyValueStore data;

			/**
			 * @brief Save all data to disk
			 */
			void SaveToFile();
	};

	/**
	 * @brief Get user's home directory path
	 * @return Path to home directory
	 */
	extern std::string GetHomePath();

	/** Global game data manager instance */
	extern GameData data;
}
#endif