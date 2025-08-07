#pragma once
#ifdef StevEngine_NETWORKING
#include "utilities/Stream.hpp"
#include <cstdint>

#ifdef _WIN32
	#include <winsock2.h>
	#include <Ws2tcpip.h>
	#pragma comment(lib, "ws2_32.lib") //Winsock Library
#else
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <unistd.h>
	#include <arpa/inet.h>
#endif

namespace StevEngine::Networking {
	//ID of which message is sent or recieved (0 and 1 are reserved for engine purposes)
	typedef uint32_t MessageID;

	struct Message {
		MessageID id;
		Utilities::Stream data;
		uint32_t size;
	};

	void initWinSock();
	#ifdef _WIN32
		WSADATA wsa;
		typedef SOCKET Socket;
	#else
		typedef int Socket;
	#endif
}
#endif