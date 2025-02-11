#pragma once
#ifdef StevEngine_PLAYER_DATA
#include "utilities/KeyValueStore.hpp"
#include "main/Log.hpp"

#include <string>

namespace StevEngine::Data {
	/**
	 * @brief Manages game settings and configuration
	 *
	 * Handles saving/loading game settings. Settings are stored
	 * in the user's config directory and persist between game sessions.
	 */
	class Settings {
		public:
			/**
			 * @brief Initialize settings manager
			 * @param title Game title used for config directory name
			 */
			void Init(std::string title);

			/**
			 * @brief Read setting value of specified type
			 * @tparam T Type of setting to read
			 * @param name Setting key name
			 * @return Value of type T
			 */
			template<typename T> T Read(std::string name) const {
				//Check if key exists
				if(!settings.Contains(name)) {
					Log::Error(std::format("Setting with key '{}' not found!", name), true);
					return (T)NULL;
				}
				//Get data
				return settings.Read<T>(name);
			}

			/**
			 * @brief Check if setting exists
			 * @param name Setting key to check
			 * @return true if setting exists, false otherwise
			 */
			bool HasValue(std::string name) const;

			/**
			 * @brief Save setting value
			 * @tparam T Type of value to save
			 * @param name Setting key name
			 * @param data Value to save
			 */
			template<typename T> void Save(std::string name, T data) {
				settings.Write(name, data);
			}

			/**
			 * @brief Delete saved setting
			 * @param name Setting key to delete
			 */
			void Delete(std::string name);

			/**
			 * @brief Get path to config directory
			 * @return Path to config directory
			 */
			std::string GetConfigPath() const { return configPath; }

			/**
			 * @brief Write all settings to disk
			 */
			void SaveToFile() const;

		private:
			std::string configPath;  ///< Path to config directory
			Utilities::KeyValueStore settings;	 ///< Key value store containing all settings
	};

	/** Global settings manager instance */
	extern Settings settings;
}
#endif