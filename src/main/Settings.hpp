#pragma once
#ifdef StevEngine_PLAYER_DATA
#include <string>
#include <map>
#include <fstream>
#include <yaml-cpp/yaml.h>

#include "main/Log.hpp"

namespace StevEngine {
	class Engine;

	class Settings {
		friend class StevEngine::Engine;
		public:
			template<typename T> T Read(std::string name)  {
				//Get line if key exists
				if(!settings[name]) {
					Log::Error(std::format("Game data with key '{}' not found!", name), true);
					return NULL;
				}
				//Get data
				return settings[name].as<T>();
			}
			bool HasValue(std::string name);
			void Save(std::string name, YAML::Node data);
			void Delete(std::string name);
			std::string GetConfigPath() { return configPath; }
		private:
			Settings(std::string title);
			std::string configPath;
			YAML::Node settings;
			void SaveToFile();
	};
}
#endif
