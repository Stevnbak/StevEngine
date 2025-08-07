#pragma once
#ifdef StevEngine_NETWORKING
#include "networking.hpp"
#include "utilities/ID.hpp"

#include <string>

namespace StevEngine::Networking::Client {

	using MessageFunction = std::function<void(const Message& message)>;

	class MessageHandler {
		public:
			MessageHandler(const MessageFunction& function);
			MessageHandler(const MessageHandler& copy);
			void operator= (const MessageHandler& copy);

			void operator() (const Message& message) const;

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

			bool send(const Message& message);

			Utilities::ID listen(MessageID id, MessageFunction function);
			void unlisten(MessageID id, const Utilities::ID handler);

		private:
			sockaddr_in serverAddress;

			std::unordered_map<MessageID, std::vector<MessageHandler>> subscribers;

			fd_set readfds;
			bool connect();
			void disconnect();
			void recieve(const Message& message);

			Socket connection;
	};
}

#endif