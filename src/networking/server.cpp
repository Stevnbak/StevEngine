#ifdef StevEngine_NETWORKING
#include "server.hpp"
#include "networking.hpp"
#include "main/Engine.hpp"
#include "main/EngineEvents.hpp"
#include "main/Log.hpp"
#include "utilities/Stream.hpp"
#include <cstdint>
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

		send(client, 0);
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
				MessageID id;
				uint32_t size;
				if(recv(client.socket, &id, sizeof(id), 0) < sizeof(id)) continue; //TODO: Handle partial messages better
				if(recv(client.socket, &size, sizeof(size), 0) < sizeof(id)) continue;

				//Read data
				Utilities::Stream stream(Utilities::Binary);
				if(size > 0) {
					char* buf = new char[size];
					if(recv(client.socket, buf, size, 0) < size) {
						delete[] buf;
						continue;
					}
					for(uint32_t i = 0; i < size; i++) stream << buf[i];
					delete[] buf;
				}
				i++;
				if(id == 1) {
					disconnected.insert(client);
					continue;
				}
				//Publish to listeners
				recieve(client, {id, stream});
			}
        }


	}

	void Manager::sendAll(const Message& message) {

	}


	void Manager::send(const Client& client, const Message& message) {
		//Create data
		Utilities::Stream raw(Utilities::Binary);
		raw << message.id << (uint32_t)message.data.GetStream().view().size() << message.data;
		//Try and send data
		::send(client.socket, raw.GetStream().view().data(), raw.GetStream().view().size(), MSG_NOSIGNAL);
	}

	void Manager::recieve(const Client& client, const Message& message) {
		auto& handlers = subscribers[message.id];
		auto i = handlers.begin();
		while(i != handlers.end()) {
			(*i)(client, message);
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

	void MessageHandler::operator() (const Client& client, Message message) const {
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
