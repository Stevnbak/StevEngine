#ifdef StevEngine_PLAYER_DATA
#include "main/Engine.hpp"
#include "Settings.hpp"
#include "DataManager.hpp"

#include <string>
#include <filesystem>
#include <algorithm>

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
		std::ifstream start;
		start.open(configPath + "game.settings");
		settings = YAML::Load(start);
	}
	bool Settings::HasValue(std::string name) const {
		return settings[name].IsDefined();
	}
	void Settings::Delete(std::string name) {
		settings.remove(name);
	}
	void Settings::SaveToFile() const {
		std::ofstream file;
		file.open(configPath + "game.settings");
		file.clear();
		YAML::Emitter out;
		out << settings;
		file << out.c_str();
		file.close();
	}
}
#endif
