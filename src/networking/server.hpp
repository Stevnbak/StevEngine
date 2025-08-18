#pragma once
#ifdef StevEngine_NETWORKING
#include "networking.hpp"
#include "utilities/ID.hpp"

#include <string>
#include <functional>
#include <unordered_set>

/**
 * @file server.hpp
 * @brief Server-side networking interfaces for hosting a StevEngine server.
 */

namespace StevEngine::Networking::Server {
	/**
	 * @brief Represents a connected client on the server
	 *
	 * Stores the socket and unique client identifier. Comparison and hashing
	 * are based on the client ID, allowing usage in unordered containers.
	 */
	class Client {
		friend class Manager;
		public:
			/** @brief Unique client identifier (assigned by the server) */
			const Utilities::ID id;

			/** @brief Socket associated with the client connection */
			const Socket socket;

			/**
			 * @brief Compare two clients by ID
			 * @param client Other client to compare with
			 * @return true if both clients have the same ID
			 */
			bool operator== (const Client& client) const;
		private:
			/**
			 * @brief Construct client with a socket, generating a new ID
			 * @param connection Accepted socket for the client
			 */
			Client(const Socket& connection);

			/**
			 * @brief Construct client with a socket and known ID
			 * @param connection Accepted socket for the client
			 * @param id Existing client identifier
			 */
			Client(const Socket& connection, const Utilities::ID& id);

			/** @brief Seconds elapsed since last ping from this client */
			mutable float sinceLastPing = 0;
	};
}

/**
 * @brief Hash specialization for Server::Client based on its ID
 */
template<> struct std::hash<StevEngine::Networking::Server::Client> {
	std::size_t operator()(const StevEngine::Networking::Server::Client& k) const {
		size_t result = 0;
		return std::hash<StevEngine::Utilities::ID>()(k.id);
	}
};

namespace StevEngine::Networking::Server {

	/**
	 * @brief Callback function type for messages received from clients
	 * @param client Source client of the message
	 * @param message Binary payload of the message
	 */
	using MessageFunction = std::function<void(const Client& client, MessageData message)>;

	/**
	 * @brief Starts and manages a networking server
	 *
	 * Listens for incoming client connections, receives and dispatches messages,
	 * and provides API to send messages to one or all clients. Integrates with
	 * the engine update loop to handle connections and timeouts without threading.
	 */
	class Manager {
		private:
			/**
			 * @brief Message received event handler
			 *
			 * Wraps a callback function and provides a unique ID to enable
			 * unsubscription of specific handlers.
			 */
			class MessageHandler {
				public:
					/**
					 * @brief Create new message handler
					 * @param function Function to execute when a message is received
					 */
					MessageHandler(const MessageFunction& function);

					/**
					 * @brief Copy-construct a message handler
					 * @param copy Other handler to copy from
					 */
					MessageHandler(const MessageHandler& copy);

					/**
					 * @brief Assign from another message handler
					 * @param copy Other handler to assign from
					 */
					void operator= (const MessageHandler& copy);

					/**
					 * @brief Invoke the handler with a client and message data
					 * @param client Source client
					 * @param message Message payload
					 */
					void operator() (const Client& client, MessageData message) const;

					/**
					 * @brief Compare handler identity
					 * @param other Handler to compare with
					 * @return true if both handlers represent the same subscription
					 */
					bool operator== (const MessageHandler& other) const;

					/**
					 * @brief Get unique handler identifier
					 * @return Unique ID of this handler
					 */
					Utilities::ID getId() const { return id; }
				private:
					MessageFunction function;  ///< Registered callback function
					Utilities::ID id;          ///< Unique handler identifier
			};
		public:
			/**
			 * @brief Create and start a new server manager
			 * @param ip IPv4/IPv6 address to bind the server to
			 * @param port Port number to listen on
			 *
			 * Initializes the server socket, binds to the given endpoint and starts
			 * listening for incoming connections. Subscribes to engine events to
			 * accept connections and receive messages.
			 * @throws std::runtime_error if the server cannot bind or listen
			 */
			Manager(std::string ip, int port);

			/**
			 * @brief Clean up server manager and close the server socket
			 */
			~Manager();

			/**
			 * @brief Send a message to all connected clients
			 * @param message Message (ID + binary payload) to send
			 */
			void sendAll(const Message& message) const;

			/**
			 * @brief Send a message with ID and optional payload to all clients
			 * @param id Message identifier
			 * @param data Optional binary payload (defaults to empty)
			 */
			void sendAll(const MessageID& id, MessageData data = MessageData()) const;

			/**
			 * @brief Send a message to a specific client
			 * @param to Destination client
			 * @param message Message (ID + binary payload) to send
			 */
			void send(const Client& to, const Message& message) const;

			/**
			 * @brief Send a message with ID and optional payload to a specific client
			 * @param to Destination client
			 * @param id Message identifier
			 * @param data Optional binary payload (defaults to empty)
			 */
			void send(const Client& to, const MessageID& id, MessageData data = MessageData()) const;

			/**
			 * @brief Send a message to a client identified by ID
			 * @param clientId Destination client's unique ID
			 * @param message Message (ID + binary payload) to send
			 * @throws std::runtime_error if the client is not currently connected
			 */
			void send(const Utilities::ID& clientId, const Message& message) const;

			/**
			 * @brief Send a message with ID and optional payload to a client by ID
			 * @param clientId Destination client's unique ID
			 * @param id Message identifier
			 * @param data Optional binary payload (defaults to empty)
			 * @throws std::runtime_error if the client is not currently connected
			 */
			void send(const Utilities::ID& clientId, const MessageID& id, MessageData data = MessageData()) const;

			/**
			 * @brief Subscribe to a message ID from any client
			 * @param id Message identifier to listen for
			 * @param function Callback to invoke when the message is received
			 * @return Subscription ID that can be used to unlisten
			 */
			Utilities::ID listen(MessageID id, MessageFunction function);

			/**
			 * @brief Unsubscribe a previously registered message handler
			 * @param id Message identifier
			 * @param handler Subscription ID returned by listen
			 */
			void unlisten(MessageID id, const Utilities::ID handler);

			/**
			 * @brief Get a readonly view of the currently connected clients
			 * @return Set of connected clients
			 */
			const std::unordered_set<Client>& getClients() const { return clients; };

		private:
			sockaddr_in serverAddress;  ///< Local server address information

			/** @brief Map of message ID to registered handlers */
			std::unordered_map<MessageID, std::vector<MessageHandler>> subscribers;

			/** @brief Set of currently connected clients */
			std::unordered_set<Client> clients;

			/** @brief Clients marked as disconnected to be removed on next update */
			std::unordered_set<Client> disconnected;

			fd_set readfds;             ///< File descriptor set used for select()

			/**
			 * @brief Accept and register any pending incoming connections
			 */
			void acceptConnections();

			/**
			 * @brief Read and dispatch messages from all connected clients
			 */
			void recieveMessages();

			/**
			 * @brief Dispatch a received message to subscribers
			 * @param from Client that sent the message
			 * @param message Message to dispatch
			 */
			void recieve(const Client& from, const Message& message);

			Socket server;              ///< Listening server socket
	};
}

#endif