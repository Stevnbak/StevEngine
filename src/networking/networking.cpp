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

	Message readMessage(Socket connection) {
		MessageID id;
		uint32_t size;
		if(recv(connection, &id, sizeof(id), 0) < sizeof(id)) return {5}; //TODO: Handle partial messages better
		if(recv(connection, &size, sizeof(size), 0) < sizeof(id)) return {5};
		//Read data
		Utilities::Stream stream(Utilities::Binary);
		if(size > 0) {
			char* buf = new char[size];
			if(recv(connection, buf, size, 0) < size) {
				delete[] buf;
				return {5};
			}
			for(uint32_t i = 0; i < size; i++) stream << buf[i];
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
}

#endif
