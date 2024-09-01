#ifdef StevEngine_PLAYER_DATA
#include "Settings.hpp"
#include "main/Engine.hpp"
#include "main/Log.hpp"
#include "DataManager.hpp"

#include <string>
#include <filesystem>
#include <algorithm>

namespace StevEngine {
        std::string GetConfPath() {
            if(std::getenv("HOME")) {
                return std::format("{}/.config/", GetHomePath());
            }
            else {
                return GetHomePath();
            }
        }
		Settings::Settings(std::string title) {
		    configPath = std::format("{}{}/", GetConfPath(), title);
			std::filesystem::create_directories(configPath);
			//Read or create data file
			std::ifstream start;
			start.open(configPath + "game.settings");
			settings = YAML::Load(start);
		}
		template<typename T> T Settings::Read(std::string name) {
			//Get line if key exists
			if(!settings[name]) {
				Log::Error(std::format("Game data with key '{}' not found!", name), true);
				return NULL;
			}
			//Get data
			return settings[name].as<T>();
		}
		void Settings::Save(std::string name, YAML::Node input) {
		    settings[name] = input;
			SaveToFile();
		}
		void Settings::Delete(std::string name) {
		    settings.remove(name);
			SaveToFile();
		}
		void Settings::SaveToFile() {
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
