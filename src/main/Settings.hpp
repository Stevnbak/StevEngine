#pragma once
#ifdef StevEngine_PLAYER_DATA
#include <string>
#include <map>
#include <fstream>
#include <yaml-cpp/yaml.h>

namespace StevEngine {
	class Engine;

	class Settings {
		friend class StevEngine::Engine;
		public:
			template<typename T> T Read(std::string name);
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
