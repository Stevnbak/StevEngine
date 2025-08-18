#pragma once
#ifdef StevEngine_NETWORKING
#include "networking.hpp"
#include "utilities/ID.hpp"

#include <string>

/**
 * @file client.hpp
 * @brief Client-side networking interfaces for connecting to a StevEngine server.
 */

namespace StevEngine::Networking::Client {

	/**
	 * @brief Callback function type for received server messages
	 * @param message Binary payload of the received message
	 */
	using MessageFunction = std::function<void(MessageData message)>;

	/**
	 * @brief Manages a client connection to a networking server
	 *
	 * Handles connection lifecycle, message send/receive and subscription of
	 * message handlers. Automatically integrates with the engine update loop
	 * to poll for incoming messages and maintain connection health (ping).
	 */
	class Manager {
		private:
			/**
			 * @brief Wrapper for a subscribed message callback
			 *
			 * Stores the callback function and a unique handler ID, allowing it to be
			 * referenced and removed later.
			 */
			class MessageHandler {
				public:
					/**
					 * @brief Create a new message handler
					 * @param function Function to execute when the message is received
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
					 * @brief Invoke the handler with message data
					 * @param message Message payload to process
					 */
					void operator() (MessageData message) const;

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
					MessageFunction function;   ///< Registered callback function
					Utilities::ID id;           ///< Unique handler identifier
			};
		public:
			/**
			 * @brief Create a new client manager and connect to the server
			 * @param ip IPv4/IPv6 address of the server
			 * @param port Port number to connect to
			 *
			 * Initializes the socket, attempts to connect and performs the initial
			 * handshake with the server. Will subscribe to engine events to process
			 * incoming messages and manage ping/timeout handling.
			 * @throws std::runtime_error if initial connection fails
			 */
			Manager(std::string ip, int port);

			/**
			 * @brief Clean up client manager
			 *
			 * Gracefully disconnects from the server and releases resources.
			 */
			~Manager();

			/**
			 * @brief Send a fully constructed message to the server
			 * @param message Message (ID + binary payload) to send
			 */
			void send(const Message& message) const;

			/**
			 * @brief Send a message with ID and optional payload to the server
			 * @param id Message identifier
			 * @param data Optional binary payload (defaults to empty)
			 */
			void send(const MessageID& id, MessageData data = MessageData()) const;

			/**
			 * @brief Subscribe to a message ID from the server
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

		private:
			sockaddr_in serverAddress;  ///< Remote server address information

			/** @brief Map of message ID to registered handlers */
			std::unordered_map<MessageID, std::vector<MessageHandler>> subscribers;

			fd_set readfds;             ///< File descriptor set used for select()
			/**
			 * @brief Attempt to connect and perform handshake with server
			 * @return true on success, false if the connection could not be established
			 */
			bool connect();

			/**
			 * @brief Gracefully disconnect from the server
			 *
			 * Sends a disconnection message and closes the socket.
			 */
			void disconnect();

			/**
			 * @brief Dispatch a received message to subscribers
			 * @param message Message received from the server
			 */
			void recieve(const Message& message);

			Socket connection;          				///< Underlying socket connection
			Utilities::ID id = Utilities::ID::empty;  	///< Client ID assigned by the server

			// Ping/timeout handling
			uint32_t pingTime = 0;      ///< Last measured ping (milliseconds)
			float sinceLastPing = 0;    ///< Seconds since last ping was received
	};
}

#endif