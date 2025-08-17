#pragma once
#include <cstdint>
#ifdef StevEngine_NETWORKING
#include "networking.hpp"
#include "utilities/ID.hpp"

#include <string>

namespace StevEngine::Networking::Client {

	using MessageFunction = std::function<void(MessageData message)>;

	class MessageHandler {
		public:
			MessageHandler(const MessageFunction& function);
			MessageHandler(const MessageHandler& copy);
			void operator= (const MessageHandler& copy);

			void operator() (MessageData message) const;

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

			void send(const Message& message) const;
			void send(const MessageID& id, MessageData data = MessageData()) const;

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
			Utilities::ID id = Utilities::ID::empty;

			//Ping
			uint32_t pingTime = 0; //in ms
			float sinceLastPing = 0;
	};
}

#endif