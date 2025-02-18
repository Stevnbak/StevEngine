#ifdef StevEngine_PLAYER_DATA
#include "Settings.hpp"
#include "DataManager.hpp"

#include <string>
#include <filesystem>
#include <fstream>

namespace StevEngine::Data {
	Settings settings = Settings();
	std::string GetConfPath() {
		if(std::getenv("HOME")) {
			return std::format("{}/.config/", GetHomePath());
		}
		else {
			return GetHomePath();
		}
	}
	void Settings::Init(std::string title) {
		configPath = std::format("{}{}/", GetConfPath(), title);
		std::filesystem::create_directories(configPath);
		//Read or create data file
		std::ifstream start(configPath + "game.settings");
		settings.ReadFromFile(start);
	}
	bool Settings::HasValue(std::string name) const {
		return settings.Contains(name);
	}
	void Settings::Delete(std::string name) {
		settings.Erase(name);
	}
	void Settings::SaveToFile() const {
		settings.WriteToFile((configPath + "game.settings").c_str());
	}
}
#endif
