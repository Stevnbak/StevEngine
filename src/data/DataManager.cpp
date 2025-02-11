#ifdef StevEngine_PLAYER_DATA
#include "DataManager.hpp"
#include "main/Log.hpp"

#include <string>
#include <fstream>
#include <filesystem>
#include <algorithm>

namespace StevEngine::Data {
	GameData data = GameData();
	std::string GetHomePath() {
		const char* homePath = std::getenv("HOME");
		const char* userPath = std::getenv("USERPROFILE");
		const char* homeDrive = std::getenv("HOMEDRIVE");
		const char* homePath2 = std::getenv("HOMEPATH");

		if (homePath != nullptr) {
			return std::string(homePath);
		}
		else if (userPath != nullptr) {
			return std::string(userPath) + "/";
		}
		else if (homeDrive != nullptr && homePath2 != nullptr) {
			return std::string(homeDrive) + std::string(homePath2);
		}
		else {
			Log::Error("Could not find home path! Using program path instead.");
			return "appdata";
		}
	}
	std::string GetDataPath() {
		if(std::getenv("HOME")) {
			return std::format("{}/.local/", GetHomePath());
		}
		else {
			return GetHomePath();
		}
	}

	void GameData::Init(std::string title) {
		//Get and create data folder
		std::replace(title.begin(), title.end(), ' ', '-');
		appdataPath = std::format("{}{}/", GetDataPath(), title);
		logPath = std::format("{}{}/{}/", GetDataPath(), title, "logs");
		std::filesystem::create_directories(appdataPath);
		std::filesystem::create_directories(logPath);
		//Read or create data file
		std::ifstream start(GetAppdataPath() + "game.data");
		data.ReadFromFile(start);
	}


	void GameData::Delete(std::string name) {
		data.Erase(name);
		SaveToFile();
	}
	void GameData::SaveToFile() {
		data.WriteToFile((appdataPath + "game.data").c_str());
	}
}
#endif
