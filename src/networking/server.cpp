#include <netinet/in.h>
#include <unordered_set>
#ifdef StevEngine_NETWORKING
#include "main/Engine.hpp"
#include "main/EngineEvents.hpp"
#include "main/Log.hpp"
#include "networking.hpp"
#include "server.hpp"
#include "utilities/ID.hpp"
#include "utilities/Stream.hpp"

#include <stdexcept>
#include <string>
#include <vector>
#ifndef _WIN32
#include <bits/types/struct_timeval.h>
#endif

namespace StevEngine::Networking::Server {

	Manager::Manager(int port, bool ipv4) {
		initWinSock();
		//Set ip and port
		serverAddress.sin_port = htons(port);
		serverAddress.sin_family = ipv4 ? AF_INET : AF_INET6;
		serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
		// Start TCP socket
		tcp = socket(serverAddress.sin_family, SOCK_STREAM, 0);
		int opt = 1;
		setsockopt(tcp, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
		if (bind(tcp, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) throw std::runtime_error("Failed to start TCP server at port " + std::to_string(port));
		// Start UDP socket
		udp = socket(serverAddress.sin_family, SOCK_DGRAM, 0);
		setsockopt(udp, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
		if (bind(udp, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) throw std::runtime_error("Failed to start UDP server at port " + std::to_string(port));
		// Allow connections
		if (::listen(tcp, 16) < 0) throw std::runtime_error("Failed to set TCP server to listen.");
		Log::Debug("Started socket server");
		//Update method
		engine->GetEvents().Subscribe<PreUpdateEvent>([this](const PreUpdateEvent& e) {
			//Disconnect the clients marked for disconnection
			for (auto& client : disconnected) {
				Log::Debug(std::string("Client (") + client.id.GetString() + ") disconnected!");
				clients.erase(client);
			}
			disconnected.clear();
			//Accept new connections
			acceptTCPConnections();
			connectConnections();
			//Recieve new messages from server
			recieveReliableMessages();
			recieveUnreliableMessages();
			//Send pings
			for (const Client& client : clients) send(client, Message(4), false);
		});
		//Ping
		listen(4, [this](const Client& client, auto _) {
			//Log::Debug("Ping from: " + std::string(client.id.GetString()));
			client.sinceLastPing = 0;
		});
		engine->GetEvents().Subscribe<UpdateEvent>([this](const UpdateEvent& e) {
			for (auto& client : clients) {
				client.sinceLastPing += e.deltaTime;
				//Log::Debug(std::string("Client (") + client.id.GetString() + ") ping time: " + std::to_string(sinceLastPing * 1000 + 0.5));

				if (client.sinceLastPing > TIMEOUT_PING) {
					Log::Debug(std::string("Client (") + client.id.GetString() + ") timed out!");
					disconnected.insert(client);
					recieve(client, 1); // Tell listeners client disconnected
				}
			}
		});
	}

	Manager::~Manager() {
		closeSocket(tcp);
		closeSocket(udp);
	}

	timeval timeout = {0, 0};

	void Manager::acceptTCPConnections() {
		fd_set* fds = &readfds;

		FD_ZERO(fds);
		FD_SET(tcp, fds);

		int activity = select(tcp + 1, fds, NULL, NULL, &timeout);
		if (activity < 0) return; //Failed to read info

		if (!FD_ISSET(tcp, fds)) return; //No new connection requests

		Socket connection = accept(tcp, (sockaddr*)NULL, NULL);
		if (connection < 0) return; //Failed to accept new connection
		Utilities::ID id;
		TCPConnections.emplace(id, connection);
		sendReliableMessage(connection, {1, id});
	}

	void Manager::connectConnections() {
		fd_set* fds = &readfds;
		// Handle TCP reconnection messages
		FD_ZERO(fds);
		Socket max = 0;
		for (const auto& [id, sock] : TCPConnections) {
			FD_SET(sock, fds);
			if (sock > max) max = sock;
		}
		int activity = select(max + 1, fds, NULL, NULL, &timeout);
		for (int i = 0; i < activity;) {
			auto it = TCPConnections.begin();
			while (it != TCPConnections.end()) {
				Socket sock = it->second;
				Utilities::ID oldId = it->first;
				it++;
				if (!FD_ISSET(sock, fds)) continue; //No messages from this client
				//Read id and data size
				Message message = readReliableMessage(sock);
				i++;
				if (message.id != 2) continue; // Ignore anything that's not a reconnection message
				Utilities::ID newId = message.data.Read<Utilities::ID>();
				//Log::Debug("Recieved reconnection id " + std::string(newId.GetString()));
				// If a client already exists with the requested id, ignore this request
				if(TCPConnections.contains(newId)) continue;
				if(clients.contains(Client(newId,0,{}))) continue;

				TCPConnections.emplace(newId, sock); // Insert new id
				TCPConnections.erase(oldId); // Remove old id
				break;
			}
		}
	}

	void Manager::recieveReliableMessages() {
		//Read messages
		fd_set* fds = &readfds;
		FD_ZERO(fds);
		Socket max = 0;
		for (const Client& client : clients) {
			FD_SET(client.tcp, fds);
			if (client.tcp > max) max = client.tcp;
		}
		int activity = select(max + 1, fds, NULL, NULL, &timeout);
		for (int i = 0; i < activity;) {
			for (const Client& client : clients) {
				if (!FD_ISSET(client.tcp, fds)) continue; //No messages from this client
				//Read id and data size
				Message message = readReliableMessage(client.tcp);
				i++;
				if (message.id == 0) continue; // Ignore errors
				if (message.id == 1) disconnected.insert(client);
				//Publish to listeners
				recieve(client, message);
			}
		}
	}

	void Manager::recieveUnreliableMessages() {
		fd_set* fds = &readfds;
		FD_ZERO(fds);
		FD_SET(udp, fds);
		int activity = select(udp + 1, fds, NULL, NULL, &timeout);
		for (int i = 0; i < activity; i++) {
			sockaddr_in clientAddress;
			Message message = readUnreliableMessage(udp, &clientAddress);
			// Connection messages
			if (message.id == 1) {
				Utilities::ID id = message.data.Read<Utilities::ID>();
				//Log::Debug("Recieved UDP connection id " + std::string(id.GetString()));
				if (!TCPConnections.contains(id)) continue;
				//Establish connection between TCP, UDP and client id
				Client client = Client(id, TCPConnections.at(id), clientAddress);
				clients.insert(client);
				TCPConnections.erase(id);
				Log::Debug(std::string("Client (") + client.id.GetString() + ") connected", true);
				recieve(client, 1);
				sendReliableMessage(client.tcp, 2);
			} else {
				const Client* client = NULL;
				for (const Client& c : clients) {
					if (
						c.address.sin_addr.s_addr == clientAddress.sin_addr.s_addr &&
						c.address.sin_port == clientAddress.sin_port &&
						c.address.sin_family == clientAddress.sin_family
					) {
						client = &c;
						break;
					}
				}
				if(!client) continue; // Message not from a recognized client

				recieve(*client, message);
			}
		}
	}

	void Manager::send(const Message& message, bool reliable) const {
		for (const auto& client : clients) send(client, message, reliable);
	}

	void Manager::send(const MessageID& id, MessageData data, bool reliable) const {
		send({id, data}, reliable);
	}

	void Manager::send(const Client& client, const Message& message, bool reliable) const {
		if(reliable) {
			sendReliableMessage(client.tcp, message);
		} else {
			sendUnreliableMessage(udp, &client.address, message);
		}
	}

	void Manager::send(const Client& to, const MessageID& id, MessageData data, bool reliable) const {
		send(to, {id, data}, reliable);
	}

	void Manager::send(const Utilities::ID& clientId, const Message& message, bool reliable) const {
		Client temp = Client(clientId, 0, {});
		if (!clients.contains(temp)) throw std::runtime_error(std::string("Client (") + clientId.GetString() + ") not connected");
		send(*clients.find(temp), message, reliable);
	}

	void Manager::send(const Utilities::ID& clientId, const MessageID& id, MessageData data, bool reliable) const {
		send(clientId, {id, data}, reliable);
	}

	void Manager::recieve(const Client& client, const Message& message) {
		auto& handlers = subscribers[message.id];
		auto i = handlers.begin();
		while (i != handlers.end()) {
			(*i)(client, message.data);
			i++;
		}
	}

	Utilities::ID Manager::listen(MessageID id, MessageFunction function) {
		auto& handlers = subscribers[id];
		handlers.emplace_back(function);
		return handlers[handlers.size() - 1].getId();
	}

	void Manager::unlisten(MessageID id, const Utilities::ID handler) {
		auto& handlers = subscribers[id];
		for (size_t i = 0; i < handlers.size(); i++) {
			if (handlers[i].getId() == handler) {
				handlers.erase(handlers.begin() + i);
				return;
			}
		}
	}

	Manager::MessageHandler::MessageHandler(const MessageFunction& function)
		: function(function) {}

	Manager::MessageHandler::MessageHandler(const MessageHandler& copy)
		: function(copy.function), id(copy.id) {}

	void Manager::MessageHandler::operator=(const MessageHandler& copy) {
		function = copy.function;
		id = copy.id;
	}

	void Manager::MessageHandler::operator()(const Client& client, MessageData message) const {
		function(client, message);
	}

	bool Manager::MessageHandler::operator==(const MessageHandler& other) const {
		return (other.id == id);
	}

	Client::Client(const Utilities::ID& id, const Socket& tcp, const sockaddr_in& address)
		: tcp(tcp), id(id), address(address) {}

	bool Client::operator==(const Client& client) const {
		return (id == client.id);
	}
}

#endif
