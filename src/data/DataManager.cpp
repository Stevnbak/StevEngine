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
		Stream stream(Text);
		stream.ReadFromFile(start);
		uint32_t keys;
		stream >> keys;
		for(uint32_t i = 0; i < keys; i++) {
			std::string key = stream.Read<std::string>();
			Stream keyStream(Text);
			std::string v;
			getline(stream.GetStream(), v, '\n');
			for(char c : v) keyStream << c;
			data.emplace(key, keyStream);
		}
	}
	template<typename T> T GameData::Read(std::string name) {
		//Get line if key exists
		if(!data.contains(name)) {
			Log::Error(std::format("Game data with key '{}' not found!", name), true);
			return T();
		}
		//Get data
		//TODO: Fix this mess with creating a new stream to read value!
		Stream stream(Text);
		stream.GetStream() << data.at(name).GetStream().str();
		return stream.Read<T>();
	}
	template std::string GameData::Read(std::string name);
	template bool GameData::Read(std::string name);
	template int GameData::Read(std::string name);
	template uint32_t GameData::Read(std::string name);
	template double GameData::Read(std::string name);
	template float GameData::Read(std::string name);

	template<typename T> void GameData::Save(std::string name, T input) {
		if(data.contains(name)) {
			data.erase(name);
		}
		Stream stream(Text);
		stream << input;
		data.emplace(name, stream);
		SaveToFile();
	}
	template void GameData::Save(std::string name, std::string input);
	template void GameData::Save(std::string name, bool input);
	template void GameData::Save(std::string name, int input);
	template void GameData::Save(std::string name, uint32_t input);
	template void GameData::Save(std::string name, double input);
	template void GameData::Save(std::string name, float input);


	void GameData::Delete(std::string name) {
		data.erase(name);
	}
	void GameData::SaveToFile() {
		Stream stream(Text);
		stream << (uint32_t)data.size();
		for(const auto& [key, str] : data) {
			stream << key << str << '\n';
		}
		stream.WriteToFile(std::string("game.data").c_str());
	}
}
#endif
