#ifdef StevEngine_NETWORKING
#include "networking.hpp"
#include "utilities/Stream.hpp"


namespace StevEngine::Networking {
	Message::Message(MessageID id) : id(id), data(Utilities::Binary) {}
	Message::Message(MessageID id, MessageData data) : id(id), data(data) {
		if(data.type != Utilities::Binary) throw std::runtime_error("Message data must be a binary stream.");
	}

	#ifdef _WIN32
	WSADATA wsa;
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

	Message readMessage(Socket connection) {
		MessageID id = 0;
		uint32_t size = 0;
		if(recv(connection, (char*) &id, sizeof(id), 0) < sizeof(id)) return {4}; //TODO: Handle partial messages better
		if(recv(connection, (char*) &size, sizeof(size), 0) < sizeof(id)) return {4};
		//Read data
		Utilities::Stream stream(Utilities::Binary);
		if(size > 0) {
			char* buf = new char[size];
			if(recv(connection, buf, size, 0) < size) {
				delete[] buf;
				return {4};
			}
			stream.GetStream().write(buf, size);
			delete[] buf;
		}

		return {id, stream};
	}

	bool sendMessage(Socket connection, Message message) {
		//Create data
		Utilities::Stream raw(Utilities::Binary);
		raw << message.id << (uint32_t)message.data.GetStream().view().size() << message.data;
		//Try and send data
		return ::send(connection, raw.GetStream().view().data(), raw.GetStream().view().size(), MSG_NOSIGNAL) > 0;
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
