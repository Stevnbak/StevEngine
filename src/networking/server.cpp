#ifdef StevEngine_NETWORKING
#include "server.hpp"
#include "networking.hpp"
#include "main/Engine.hpp"
#include "main/EngineEvents.hpp"
#include "main/Log.hpp"
#include "utilities/Stream.hpp"

#include <vector>
#include <stdexcept>
#include <string>
#include <bits/types/struct_timeval.h>

namespace StevEngine::Networking::Server {

	Manager::Manager(std::string ip, int port) {
		initWinSock();
		//Set ip and port
		serverAddress.sin_port = htons(port);
		serverAddress.sin_family = AF_INET;
		if(inet_pton(AF_INET, ip.c_str(), &serverAddress.sin_addr) < 0) {
			serverAddress.sin_family = AF_INET6;
			inet_pton(AF_INET6, ip.c_str(), &serverAddress.sin_addr);
		};
		//Start socket
		server = socket(serverAddress.sin_family, SOCK_STREAM, 0);
		if(bind(server, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) throw std::runtime_error("Failed to start server at " + ip + ":" + std::to_string(port));
		//Allow connections
		if (::listen(server, 16) < 0) throw std::runtime_error("Failed to set server to listen.");
		Log::Debug("Started socket server");
		//Update method
		engine->GetEvents()->Subscribe<PreUpdateEvent>([this](const PreUpdateEvent& e) {
			//Accept new connections
			acceptConnections();
			//Recieve new messages from server
			recieveMessages();
		});
		//Reconnection (with client id)
		listen(0, [this](const Client& client, MessageData message) {
			auto id = message.Read<Utilities::ID>();
			clients.emplace(client.socket, id);
			clients.erase(client);
		});
		//Ping
		listen(3, [this](const Client& client, auto _) {
			//Log::Debug("Ping from: " + std::string(client.id.GetString()));
			client.sinceLastPing = 0;
			send(client, 3);
		});
		engine->GetEvents()->Subscribe<UpdateEvent>([this](const UpdateEvent& e) {
			for(auto& client : clients) {
				client.sinceLastPing += e.deltaTime;

				if(client.sinceLastPing > TIMEOUT_PING) {
					Log::Debug(std::string("Client (") + client.id.GetString() + ") timed out!");
					disconnected.insert(client);
				}
			}
		});
	}

	Manager::~Manager() {
		::close(server);
	}

	timeval timeout = {0, 0};


	void Manager::acceptConnections() {
		//(https://www.geeksforgeeks.org/cpp/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/)
		FD_ZERO(&readfds);
		FD_SET(server, &readfds);

		int activity = select(server + 1, &readfds, NULL, NULL, &timeout);
        if (activity<0) return; //Failed to read info

        if (!FD_ISSET(server, &readfds)) return; //No new connection requests

		Socket connection = accept(server, (sockaddr*)NULL, NULL);
		if(connection < 0) return; //Failed to accept new connection

		Log::Debug("Client connected");
		Client client(connection);
		clients.insert(client);

		send(client, 0, client.id);
	}
	void Manager::recieveMessages() {
		//Disconnect the clients marked for disconnection
		for(auto& client : disconnected) {
			Log::Debug(std::string("Client (") + client.id.GetString() + ") disconnected!");
			clients.erase(client);
		}
		disconnected.clear();

		//Read messages
		FD_ZERO(&readfds);
		Socket max = 0;
		for(auto& client : clients) {
			FD_SET(client.socket, &readfds);
			if(client.socket > max) max = client.socket;
		}
		int activity = select(max + 1, &readfds, NULL, NULL, &timeout);
        for(int i = 0; i < activity;) {
	        for(auto& client : clients) {
				if(!FD_ISSET(client.socket, &readfds)) continue; //No messages from this client
				//Read id and data size
				Message message = readMessage(client.socket);
				i++;
				if(message.id == 2) continue;
				if(message.id == 1) {
					disconnected.insert(client);
					continue;
				}
				//Publish to listeners
				recieve(client, message);
			}
        }


	}

	void Manager::sendAll(const Message& message) const {
		for(const auto& client : clients) send(client, message);
	}
	void Manager::sendAll(const MessageID& id, MessageData data) const {
		sendAll({id, data});
	}

	void Manager::send(const Client& client, const Message& message) const {
		sendMessage(client.socket, message);
	}
	void Manager::send(const Client& to, const MessageID& id, MessageData data) const {
		send(to, {id, data});
	}

	void Manager::recieve(const Client& client, const Message& message) {
		auto& handlers = subscribers[message.id];
		auto i = handlers.begin();
		while(i != handlers.end()) {
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
		for(size_t i = 0; i < handlers.size(); i++) {
			if (handlers[i].getId() == handler) {
				handlers.erase(handlers.begin() + i);
				return;
			}
		}
	}

	MessageHandler::MessageHandler(const MessageFunction& function) : function(function) {}
	MessageHandler::MessageHandler(const MessageHandler& copy) : function(copy.function), id(copy.id) {}
	void MessageHandler::operator= (const MessageHandler& copy) {
		function = copy.function;
		id = copy.id;
	}

	void MessageHandler::operator() (const Client& client, MessageData message) const {
		function(client, message);
	}

	bool MessageHandler::operator== (const MessageHandler& other) const {
		return (other.id == id);
	}

	Client::Client(const Socket& connection) : socket(connection) {}
	Client::Client(const Socket& connection, const Utilities::ID& id) : socket(connection), id(id) {}

	bool Client::operator== (const Client& client) const {
		return (id == client.id);
	}
}

#endif
