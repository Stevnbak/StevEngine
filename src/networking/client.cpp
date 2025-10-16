#ifdef StevEngine_NETWORKING
#include "client.hpp"
#include "networking.hpp"
#include "main/Engine.hpp"
#include "main/EngineEvents.hpp"
#include "main/Log.hpp"
#include "utilities/Stream.hpp"

#include <vector>
#include <stdexcept>

namespace StevEngine::Networking::Client {
	timeval timeout = {0, 0};

	Manager::Manager(std::string ip, int port) {
		if(ip == "localhost") ip = "127.0.0.1";
		initWinSock();
		//Set ip and port
		serverAddress.sin_port = htons(port);
		serverAddress.sin_family = AF_INET;
		if(inet_pton(AF_INET, ip.c_str(), &serverAddress.sin_addr) < 0) {
			serverAddress.sin_family = AF_INET6;
			inet_pton(AF_INET6, ip.c_str(), &serverAddress.sin_addr);
		};

		//Setup listeners
		engine->GetEvents().Subscribe<PreUpdateEvent>([this](const PreUpdateEvent& e) {
			//Read all new messages from TCP server
			while(true) {
				FD_ZERO(&readfds);
				FD_SET(tcp, &readfds);
				int activity = select(tcp + 1, &readfds, NULL, NULL, &timeout);
	        	if (activity < 0) break; //Failed to read info
	         	if (!FD_ISSET(tcp, &readfds)) break; //No new messages
				//Read id and data size
				Message message = readReliableMessage(tcp);
				if(message.id == 0) break; //Error occured, ignore message
				if(!isConnected) {
					if(message.id == 1) {
						if(id == Utilities::ID::empty) {
							//First connection, save given id
							id = message.data.Read<Utilities::ID>();
							//Log::Debug("Recieved id from server: " + std::string(id.GetString()));
							sendUnreliableMessage(udp, NULL, {1, id});
						} else {
							//Reconnection
							sendReliableMessage(tcp, {2, id});
							//Log::Debug("Sent reconnection message to server with id " + std::string(id.GetString()));
						}
					} else if(message.id == 2) {
						Log::Debug("Succesfully connected to server");
						isConnected = true;
					}
				}
				//Publish to listeners
				recieve(message);
			}
			//Read all new messages from UDP server
			while(isConnected) {
				FD_ZERO(&readfds);
				FD_SET(udp, &readfds);
				int activity = select(udp + 1, &readfds, NULL, NULL, &timeout);
	        	if (activity < 0) break; //Failed to read info
	         	if (!FD_ISSET(udp, &readfds)) break; //No new messages
				//Read id and data size
				Message message = readUnreliableMessage(udp, NULL);
				if(message.id == 0) break; //Error occured, ignore message
				//Publish to listeners
				recieve(message);
			}
			//Send ping or connection id every update
			if(isConnected) send(Message(4), false);
			else if(id != Utilities::ID::empty) send(Message(1, id), false);
		});
		engine->GetEvents().Subscribe<UpdateEvent>([this](const UpdateEvent& e) {
			if(!isConnected) return;
			sinceLastPing += e.deltaTime;
			//Log::Debug("Ping time: " + std::to_string(sinceLastPing * 1000 + 0.5));

			if(sinceLastPing > TIMEOUT_PING) {
				Log::Debug("Server timed out!");
				sinceLastPing = 0;
				isConnected = false;
				closeSocket(tcp);
				closeSocket(udp);
				//TODO: Currently whole program stops and waits for a new connection, make it so it just tries every update
				connect();
			}
		});
		//Ping
		listen(4, [this](auto _) {
			//Log::Debug("Ping: " + std::to_string(sinceLastPing * 1000 + 0.5));
			sinceLastPing = 0;
		});
		//Start connection
		if(!connect()) throw std::runtime_error("Failed to connect to server at " + ip + ":" + std::to_string(port));
	}

	bool Manager::connect() {
		//Connect to server
		tcp = socket(serverAddress.sin_family, SOCK_STREAM, 0);
		udp = socket(serverAddress.sin_family, SOCK_DGRAM, 0);
		if(::connect(tcp, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) return false;
		if(::connect(udp, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) return false;

		return true;
	}

	void Manager::disconnect() {
		send((Message){3}, true);
		closeSocket(tcp);
		closeSocket(udp);
		id = Utilities::ID::empty;
		isConnected = false;
		sinceLastPing = 0;
	}

	Manager::~Manager() {
		disconnect();
	}

	void Manager::send(const Message& message, bool reliable) const {
		if(reliable) sendReliableMessage(tcp, message);
		else sendUnreliableMessage(udp, NULL, message);
	}
	void Manager::send(const MessageID& id, MessageData data, bool reliable) const {
		send({id, data}, reliable);
	}

	void Manager::recieve(const Message& message) {
		auto& handlers = subscribers[message.id];
		auto i = handlers.begin();
		while(i != handlers.end()) {
			(*i)(message.data);
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

	Manager::MessageHandler::MessageHandler(const MessageFunction& function) : function(function) {}
	Manager::MessageHandler::MessageHandler(const MessageHandler& copy) : function(copy.function), id(copy.id) {}
	void Manager::MessageHandler::operator= (const MessageHandler& copy) {
		function = copy.function;
		id = copy.id;
	}

	void Manager::MessageHandler::operator() (MessageData message) const {
		function(message);
	}

	bool Manager::MessageHandler::operator== (const MessageHandler& other) const {
		return (other.id == id);
	}
}

#endif
