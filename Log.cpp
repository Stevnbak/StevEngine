#include <iostream>
#include "Log.hpp"

namespace Log {
	//Engine log?
	bool engineLogEnabled = true;
	//Log functions
	void Normal(std::string msg, bool fromEngine) {
		if (!engineLogEnabled && fromEngine) return;
		std::cout << "\033[0;37m[INFO] - " << msg << "\033[0;0m" << std::endl;
	}
	void Error(std::string msg, bool fromEngine) {
		std::string start = "\033[4;31m" + std::string((!engineLogEnabled && fromEngine) ? "[ENGINE " : "[") + "ERROR] - ";
		std::cout << start << msg << "\033[0;0m" << std::endl;
	}
	void Warning(std::string msg, bool fromEngine) {
		if (!engineLogEnabled && fromEngine) return;
		std::cout << "\033[0;33m[WARNING] - " << msg << "\033[0;0m" << std::endl;
	}
}