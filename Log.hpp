#pragma once
#include <string>
namespace Log {
	void Normal(const char* msg);
	void Normal(std::string msg);
	void Error(const char* msg);
	void Error(std::string msg);
	void Warning(const char* msg);
	void Warning(std::string msg);
}