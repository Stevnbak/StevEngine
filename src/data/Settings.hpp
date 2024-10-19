#pragma once
#ifdef StevEngine_PLAYER_DATA
#include <string>
#include <map>
#include <fstream>
#include <yaml-cpp/yaml.h>

#include "main/Log.hpp"

namespace StevEngine::Data {
	class Settings {
		public:
			void Init(std::string title);
			template<typename T> T Read(std::string name) const {
				//Get line if key exists
				if(!settings[name]) {
					Log::Error(std::format("Game data with key '{}' not found!", name), true);
					return NULL;
				}
				//Get data
				return settings[name].as<T>();
			}
			bool HasValue(std::string name) const;
			template<typename T> void Save(std::string name, T data) {
				settings[name] = data;
			}
			void Delete(std::string name);
			std::string GetConfigPath() const { return configPath; }
			void SaveToFile() const;
		private:
			std::string configPath;
			YAML::Node settings;
	};

	extern Settings settings;
}
#endif
