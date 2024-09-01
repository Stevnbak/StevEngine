#pragma once
#ifdef StevEngine_PLAYER_DATA
#include <string>
#include <map>
#include <fstream>
#include <yaml-cpp/yaml.h>

namespace StevEngine {
	class Engine;

	class GameData {
		friend class StevEngine::Engine;
		public:
			template<typename T> T Read(std::string name);
			template<typename T> void Save(std::string name, T data);
			void Delete(std::string name);
			std::string GetAppdataPath() { return appdataPath; }
			std::string GetLogPath() { return logPath; }
		private:
			GameData(std::string title);
			std::string appdataPath;
			std::string logPath;
			YAML::Node data;
			void SaveToFile();
	};

	extern std::string GetHomePath();
}
#endif
