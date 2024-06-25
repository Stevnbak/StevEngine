#include "Log.hpp"
#include "Engine.hpp"

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
	void StartLogging(std::string path) {
		std::filesystem::create_directories(path + "logs");
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		std::string logFilePath = std::format("{}logs/{}.log", path, std::format("{}.{}.{}-{}.{}.{}", now->tm_year + 1900, now->tm_mon, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec));
		logFile.open(logFilePath);
		Normal("Created log file at path: " + logFilePath, true);
	}

	void CloseLogging() {
		logFile.close();
	}

	//Log functions
	void Normal(std::string msg, bool fromEngine) {
		if (!engineLogEnabled && fromEngine) return;
		std::string str = std::string(fromEngine ? "[ENGINE] " : "") + "[INFO] - " + msg;
		std::cout << "\033[0;37m" << str << "\033[0m" << std::endl;
		logFile << str << std::endl;
	}
	void Debug(std::string msg, bool fromEngine) {
		if (!engineLogEnabled && fromEngine) return;
		std::string str = std::string(fromEngine ? "[ENGINE] " : "") + "[DEBUG] - " + msg;
		std::cout << "\033[0;94m" << str << "\033[0m" << std::endl;
		logFile << str << std::endl;
	}
	void Error(std::string msg, bool fromEngine) {
		std::string str = std::string(fromEngine ? "[ENGINE] " : "") + "[ERROR] - " + msg;
		std::cerr << "\033[4;31m" << str << "\033[0m" << std::endl;
		logFile << str << std::endl;
	}
	void Warning(std::string msg, bool fromEngine) {
		if (!engineLogEnabled && fromEngine) return;
		std::string str = std::string(fromEngine ? "[ENGINE] " : "") + "[WARNING] - " + msg;
		std::cout << "\033[0;33m" << str << "\033[0m" << std::endl;
		logFile << str << std::endl;
	}
}