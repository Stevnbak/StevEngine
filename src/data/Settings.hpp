#pragma once
#ifdef StevEngine_PLAYER_DATA
#include <string>
#include <map>
#include <fstream>
#include <yaml-cpp/yaml.h>

#include "main/Log.hpp"

namespace StevEngine::Data {
	/**
	 * @brief Manages game settings and configuration
	 *
	 * Handles saving/loading game settings using YAML format. Settings are stored
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
			 * @throws Error if setting doesn't exist
			 */
			template<typename T> T Read(std::string name) const {
				//Get line if key exists
				if(!settings[name]) {
					Log::Error(std::format("Game data with key '{}' not found!", name), true);
					return NULL;
				}
				//Get data
				return settings[name].as<T>();
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
				settings[name] = data;
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
			YAML::Node settings;	 ///< YAML node containing all settings
	};

	/** Global settings manager instance */
	extern Settings settings;
}
#endif