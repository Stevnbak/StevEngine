#ifdef StevEngine_NETWORKING
#include "networking.hpp"
#include "utilities/Stream.hpp"
#include <stdexcept>

namespace StevEngine::Networking {
	Message::Message(MessageID id) : id(id), data(Utilities::Binary) {}
	Message::Message(MessageID id, Utilities::Stream data) : id(id), data(data) {
		if(data.type != Utilities::Binary) throw std::runtime_error("Message data must be a binary stream.");
	}

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
