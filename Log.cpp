#include <iostream>
#include "Log.hpp"

namespace Log {
	void Normal(const char* msg) {
		std::cout << "[INFO] - " << msg << std::endl;
	}
	void Normal(std::string msg) {
		std::cout << "[INFO] - " << msg << std::endl;
	}
	void Error(const char* msg) {
		std::cout << "[ERROR] - " << msg << std::endl;
	}
	void Error(std::string msg) {
		std::cout << "[ERROR] - " << msg << std::endl;
	}
	void Warning(const char* msg) {
		std::cout << "[WARNING] - " << msg << std::endl;
	}
	void Warning(std::string msg) {
		std::cout << "[WARNING] - " << msg << std::endl;
	}
}