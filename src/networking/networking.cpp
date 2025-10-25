#ifdef StevEngine_NETWORKING
#include "networking.hpp"
#include "utilities/Stream.hpp"

namespace StevEngine::Networking {
	Message::Message(MessageID id)
		: id(id), data(Utilities::Binary) {}

	Message::Message(MessageID id, MessageData data)
		: id(id), data(data) {
		if (data.type != Utilities::Binary) throw std::runtime_error("Message data must be a binary stream.");
	}

#ifdef _WIN32
	WSADATA wsa;
	bool winsockInitalized = false;

	void initWinSock() {
		if (winsockInitalized) return;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
			throw std::runtime_error("Failed to initialize WinSock");
		}
		winsockInitalized = true;
	}
#else
	void initWinSock() {}
#endif

	Message readReliableMessage(Socket connection) {
		MessageID id = 0;
		uint32_t size = 0;
		if (recv(connection, (char*)&id, sizeof(id), MSG_WAITALL) < sizeof(id)) return {0}; //TODO: Handle partial messages better
		if (recv(connection, (char*)&size, sizeof(size), MSG_WAITALL) < sizeof(size)) return {0};
		//Read data
		Utilities::Stream stream(Utilities::Binary);
		if (size > 0) {
			char* buf = new char[size];
			if (recv(connection, buf, size, MSG_WAITALL) < size) {
				delete[] buf;
				return {0};
			}
			stream.GetStream().write(buf, size);
			delete[] buf;
		}

		return {id, stream};
	}

	bool sendReliableMessage(Socket connection, Message message) {
		//Create data
		Utilities::Stream raw(Utilities::Binary);
		raw << message.id << (uint32_t)message.data.GetStream().view().size() << message.data;
		//Try and send data
		return ::send(connection, raw.GetStream().view().data(), raw.GetStream().view().size(), MSG_NOSIGNAL) > 0;
	}

#define MAX_UDP_SIZE (65535)
	char udpBuffer[MAX_UDP_SIZE];

	size_t baseSize = sizeof(MessageID) + sizeof(uint32_t);

	Message readUnreliableMessage(Socket connection, sockaddr_in* address) {
		socklen_t addrLen = sizeof(*address);
		uint32_t msgSize = recvfrom(connection, udpBuffer, MAX_UDP_SIZE, 0, (sockaddr*)address, &addrLen);
		if (msgSize < baseSize) return {0};

		MessageID id = *((MessageID*)udpBuffer);
		uint32_t size = *((uint32_t*)&udpBuffer[sizeof(MessageID)]);

		Utilities::Stream stream(Utilities::Binary);
		if (msgSize < baseSize + size) return {0};
		if (size > 0) stream.GetStream().write(&udpBuffer[baseSize], size);

		return {id, stream};
	}

	bool sendUnreliableMessage(Socket connection, const sockaddr_in* address, Message message) {
		//Create data
		Utilities::Stream raw(Utilities::Binary);
		raw << message.id << (uint32_t)message.data.GetStream().view().size() << message.data;
		//Try and send data
		return ::sendto(connection, raw.GetStream().view().data(), raw.GetStream().view().size(), MSG_NOSIGNAL, (sockaddr*)address, sizeof(*address)) > 0;
	}

	void closeSocket(Socket connection) {
#ifdef _WIN32
		closesocket(connection);
#else
		close(connection);
#endif
	}
}

#endif
