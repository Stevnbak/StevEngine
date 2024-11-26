#pragma once
#include <string>
#include <format>

namespace Log {
	void Normal(std::string msg, bool fromEngine = false);
	void Debug(std::string msg, bool fromEngine = false);
	void Error(std::string msg, bool fromEngine = false);
	void Warning(std::string msg, bool fromEngine = false);
	extern bool engineLogEnabled;
	#ifdef StevEngine_PLAYER_DATA
	void StartLogging(std::string path);
	void CloseLogging();
	#endif
}