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

/**
 * @brief Defines the time (in seconds) to allow between pings before disconnecting.
 */
#ifndef TIMEOUT_PING
#define TIMEOUT_PING 2
#endif

namespace StevEngine::Networking {
	/**
	 * ID of which message is sent or recieved (0 to 4 are reserved for engine purposes)
	 * 0 = Error
	 * 1 = Connection
	 * 2 = Reconnection
	 * 3 = Disconnection
	 * 4 = Ping
	 */
	typedef uint32_t MessageID;

	#define AVAILABLE_MESSAGE_ID 5

	/**
	 * Message content is a binary stream, which all StevEngine utilities and objects support being converted to
	 */
	typedef Utilities::Stream MessageData;

	/**
 	 * @brief Message contains a message ID and a message data, and fully describes the content that to be sent or recieved.
	 */
	struct Message {
		/**
		 * @brief Create a new message with no data
		 * @param id The id of the message to create
		 */
		Message(MessageID id);
		/**
		 * @brief Create a new message with data
		 * @param id The id of the message to create
		 * @param data The binary stream containing the message data
		 */
		Message(MessageID id, MessageData data);

		const MessageID id; //< id of the message
		MessageData data; //< data contents of the message
	};

	/**
	 * @brief Used internally to initialize WinSock on Windows, and does nothing on Linux
	 */
	void initWinSock();

	/**
	 * @brief Socket object type is different across platforms, this is the shared object
	 */
	#ifdef _WIN32
		extern WSADATA wsa;
		typedef SOCKET Socket;
		#define MSG_NOSIGNAL 0
	#else
		typedef int Socket;
	#endif

	/**
	 * @brief Used internally to read and decode a TCP message from a socket connection
	 *
	 * If there is no messages already on the socket, this will pause the program until there is.
	 */
	Message readReliableMessage(Socket connection);
	/**
	 * @brief Used internally to encode and send a TCP message to a socket connection
	 */
	bool sendReliableMessage(Socket connection, Message message);
	/**
	 * @brief Used internally to read and decode a UDP message from a socket connection
	 *
	 * If there is no messages already on the socket, this will pause the program until there is.
	 */
	Message readUnreliableMessage(Socket connection, sockaddr_in* address);
	/**
	 * @brief Used internally to encode and send a UDP message to a socket connection
	 */
	bool sendUnreliableMessage(Socket connection, const sockaddr_in* address, Message message);
	/**
	 * @brief Close the socket connection
	*/
	void closeSocket(Socket connection);
}
#endif