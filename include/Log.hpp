#pragma once
#include <string>
#include <format>
namespace Log {
	void Normal(std::string msg, bool fromEngine = false);
	void Error(std::string msg, bool fromEngine = false);
	void Warning(std::string msg, bool fromEngine = false);
	extern bool engineLogEnabled;
	void StartLogging(std::string gameTitle);
	void CloseLogging();
}