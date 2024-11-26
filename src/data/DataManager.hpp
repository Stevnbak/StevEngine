#pragma once
#ifdef StevEngine_PLAYER_DATA
#include <string>
#include <yaml-cpp/yaml.h>

namespace StevEngine::Data {
	class GameData {
		public:
			void Init(std::string title);
			template<typename T> T Read(std::string name);
			template<typename T> void Save(std::string name, T data);
			void Delete(std::string name);
			std::string GetAppdataPath() { return appdataPath; }
			std::string GetLogPath() { return logPath; }
		private:
			std::string appdataPath;
			std::string logPath;
			YAML::Node data;
			void SaveToFile();
	};

	extern std::string GetHomePath();

	extern GameData data;
}
#endif
