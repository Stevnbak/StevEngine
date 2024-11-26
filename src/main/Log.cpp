#include "Log.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <ctime>

namespace Log {
	//Engine log?
	bool engineLogEnabled = true;

	//Create log stream
	#ifdef StevEngine_PLAYER_DATA
	std::ofstream logFile;
	void StartLogging(std::string path) {
		std::filesystem::create_directories(path);
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		std::string logFilePath = path + std::format("log_{}.{}.{}-{}.{}.{}.log", now->tm_year + 1900, now->tm_mon, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
		logFile.open(logFilePath);
		Normal("Created log file at path: " + logFilePath, true);
	}
	void CloseLogging() {
		logFile.close();
	}
	#endif
	//Log functions
	void Normal(std::string msg, bool fromEngine) {
		if (!engineLogEnabled && fromEngine) return;
		std::string str = std::string(fromEngine ? "[ENGINE] " : "") + "[INFO] - " + msg;
		#ifdef StevEngine_DEBUGGING
		std::cout << "\033[0;37m" << str << "\033[0m" << std::endl;
		#endif
		#ifdef StevEngine_PLAYER_DATA
		logFile << str << std::endl;
		#endif
	}
	void Debug(std::string msg, bool fromEngine) {
		#ifdef StevEngine_DEBUGGING
		if (!engineLogEnabled && fromEngine) return;
		std::string str = std::string(fromEngine ? "[ENGINE] " : "") + "[DEBUG] - " + msg;
		std::cout << "\033[0;94m" << str << "\033[0m" << std::endl;
		#ifdef StevEngine_PLAYER_DATA
		logFile << str << std::endl;
		#endif
		#endif
	}
	void Error(std::string msg, bool fromEngine) {
		std::string str = std::string(fromEngine ? "[ENGINE] " : "") + "[ERROR] - " + msg;
		#ifdef StevEngine_DEBUGGING
		std::cerr << "\033[4;31m" << str << "\033[0m" << std::endl;
		#endif
		#ifdef StevEngine_PLAYER_DATA
		logFile << str << std::endl;
		#endif
	}
	void Warning(std::string msg, bool fromEngine) {
		if (!engineLogEnabled && fromEngine) return;
		std::string str = std::string(fromEngine ? "[ENGINE] " : "") + "[WARNING] - " + msg;
		#ifdef StevEngine_DEBUGGING
		std::cout << "\033[0;33m" << str << "\033[0m" << std::endl;
		#endif
		#ifdef StevEngine_PLAYER_DATA
		logFile << str << std::endl;
		#endif
	}
}
