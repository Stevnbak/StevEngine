#include "Log.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <ctime>
#include <algorithm>
#include <filesystem>

namespace Log {
	//Engine log?
	bool engineLogEnabled = true;
	
	//Create log stream
	std::ofstream logFile;
	void StartLogging(std::string gameTitle) {
		std::replace(gameTitle.begin(), gameTitle.end(), ' ', '-');
		std::string localPath = std::format("{}/{}", "appdata", gameTitle);
		std::cout << "Path: " << localPath << std::endl;
		std::filesystem::create_directories(localPath + "/logs");
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		std::string logFilePath = std::format("{}/logs/{}.log", localPath, std::format("{}.{}.{}-{}.{}.{}", now->tm_year + 1900, now->tm_mon, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec));
		logFile.open(logFilePath);
		Normal("Created log file at path: " + logFilePath);
	}

	void CloseLogging() {
		logFile.close();
	}

	//Log functions
	void Normal(std::string msg, bool fromEngine) {
		if (!engineLogEnabled && fromEngine) return;
		std::string str = "[INFO] - " + msg;
		std::cout << "\033[0;37m" << str << "\033[0;0m" << std::endl;
		logFile << str << std::endl;
	}
	void Error(std::string msg, bool fromEngine) {
		std::string str = std::string((!engineLogEnabled && fromEngine) ? "[ENGINE " : "[") + "ERROR] - " + msg;
		std::cerr << "\033[4;31m" << str << "\033[0;0m" << std::endl;
		logFile << str << std::endl;
	}
	void Warning(std::string msg, bool fromEngine) {
		if (!engineLogEnabled && fromEngine) return;
		std::string str = "[WARNING] - " + msg;
		std::cout << "\033[0;33m" << str << "\033[0;0m" << std::endl;
		logFile << str << std::endl;
	}
}