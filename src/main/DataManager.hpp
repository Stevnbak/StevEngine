#pragma once
#include <string>
#include <map>
#include <fstream>

namespace StevEngine {
	class Engine;
	namespace GameData {
		class System {
			friend class StevEngine::Engine;
			public:
				std::string ReadData(std::string name);
				void SaveData(std::string name, std::string data);
				void DeleteData(std::string name);
                std::string GetDirectoryPath() { return directoryPath; }
			private:
				System(std::string title);
                std::string directoryPath;
                std::map<std::string, std::string> data;
				void SaveDataToFile();
		};
	}
}