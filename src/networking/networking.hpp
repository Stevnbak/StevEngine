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
#define TIMEOUT_PING 1
#endif

namespace StevEngine::Networking {
	/**
	 * ID of which message is sent or recieved (0 to 4 are reserved for engine purposes)
	 * 0 = Connection
	 * 1 = Disconnection
	 * 2 = Reconnection
	 * 3 = Ping
	 * 4 = Error
	 */
	typedef uint32_t MessageID;

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
	 * @brief Used internally to read and decode a message from a socket connection
	 *
	 * If there is no message already on the socket, this will pause the program until there is.
	 */
	Message readMessage(Socket connection);
	/**
	 * @brief Used internally to encode and send a message to a socket connection
	 */
	bool sendMessage(Socket connection, Message message);
	/**
	 * @brief Close the socket connection
	*/
	void closeSocket(Socket connection);
}
#endif