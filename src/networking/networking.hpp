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

#ifndef TIMEOUT_PING
#define TIMEOUT_PING 2
#endif

namespace StevEngine::Networking {
	//ID of which message is sent or recieved (0 to 5 are reserved for engine purposes)
	// 0 = Connection
	// 1 = Disconnection
	// 2 = Error
	// 3 = Ping
	typedef uint32_t MessageID;

	typedef Utilities::Stream MessageData;


	struct Message {
		Message(MessageID id);
		Message(MessageID id, MessageData data);

		const MessageID id;
		MessageData data;
	};

	void initWinSock();
	#ifdef _WIN32
		WSADATA wsa;
		typedef SOCKET Socket;
	#else
		typedef int Socket;
	#endif

	Message readMessage(Socket connection);
	bool sendMessage(Socket connection, Message message);
}
#endif