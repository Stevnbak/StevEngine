#ifdef StevEngine_NETWORKING
#include "networking.hpp"

namespace StevEngine::Networking {
	#ifdef _WIN32
	bool winsockInitalized = false;
	void initWinSock() {
		if(winsockInitalized) return;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
			throw std::runtime_error("Failed to initialize WinSock");
		}
		winsockInitalized = true;
	}
	#else
	void initWinSock() {}
	#endif
}

#endif
