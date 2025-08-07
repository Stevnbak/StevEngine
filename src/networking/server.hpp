#pragma once
#ifdef StevEngine_NETWORKING
#include "networking.hpp"
#include "utilities/ID.hpp"

#include <string>
#include <functional>
#include <stdint.h>
#include <unordered_set>

namespace StevEngine::Networking::Server {
	class Client {
		public:
			Client(const Socket& connection);
			Client(const Socket& connection, const Utilities::ID& id);
			const Utilities::ID id;

			const Socket socket;

			bool operator== (const Client& client) const;
	};
}
template<> struct std::hash<StevEngine::Networking::Server::Client> {
	std::size_t operator()(const StevEngine::Networking::Server::Client& k) const {
		size_t result = 0;
		return std::hash<StevEngine::Utilities::ID>()(k.id);
	}
};

namespace StevEngine::Networking::Server {

	using MessageFunction = std::function<void(const Client& client, const Message& message)>;

	class MessageHandler {
		public:
			MessageHandler(const MessageFunction& function);
			MessageHandler(const MessageHandler& copy);
			void operator= (const MessageHandler& copy);

			void operator() (const Client& client, const Message& message) const;
			bool operator== (const MessageHandler& other) const;
			Utilities::ID getId() const { return id; }
		private:
			MessageFunction function;
			Utilities::ID id;
	};

	class Manager {
		public:
			Manager(std::string ip, int port);

			~Manager();

			bool sendAll(const Message& message);
			bool send(const Client& to, const Message& message);

			Utilities::ID listen(MessageID id, MessageFunction function);
			void unlisten(MessageID id, const Utilities::ID handler);

		private:
			sockaddr_in serverAddress;

			std::unordered_map<MessageID, std::vector<MessageHandler>> subscribers;
			std::unordered_set<Client> clients;

			fd_set readfds;
			void acceptConnections();
			void recieveMessages();


			void recieve(const Client& from, const Message& message);

			Socket server;
	};
}

#endif