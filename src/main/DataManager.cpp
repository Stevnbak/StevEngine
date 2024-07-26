#include "DataManager.hpp"
#include "main/Engine.hpp"
#include "main/Log.hpp"

#include <string>
#include <filesystem>
#include <algorithm>

namespace StevEngine {
    namespace GameData {
        System::System(std::string title) {
            //Get and create data folder
            std::replace(title.begin(), title.end(), ' ', '-');
            directoryPath = std::format("{}/{}/", "appdata", title);
            std::filesystem::create_directories(directoryPath);
            //Read or create data file
            std::ifstream start;
            start.open(directoryPath + "gamedata");
            std::string line;
            while (std::getline(start, line)) {
                if(line.find("=") == 0) continue;
                int split = line.find("=");
                data.insert(std::make_pair(line.substr(0, split), line.substr(split + 1, line.length() - 1).c_str()));
            }
        }
        std::string System::ReadData(std::string name) {
            //Get line if key exists
            if(!data.contains(name)) {
                Log::Error(std::format("Game data with key '{}' not found!", name), true);
                return "";
            }
            //Get data
            return data.at(name);
        }
        void System::SaveData(std::string name, std::string input) {
            if(!data.contains(name)) {
                //no data exists insert and write to file
                data.insert(std::make_pair(name, input));
            }
            else {
                //change current data values
                data[name] = input;
            }
            SaveDataToFile();
        }
        void System::DeleteData(std::string name) {
            data.erase(name);
        }
        void System::SaveDataToFile() {
            std::ofstream file;
            file.open(directoryPath + "gamedata");
            file.clear();
            for(std::pair<std::string, std::string> pair : data) {
                file << std::format("{}={}", pair.first, pair.second) << std::endl;
            }
            file.close();
        }
    }
}