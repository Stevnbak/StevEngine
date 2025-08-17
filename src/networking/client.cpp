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
		initWinSock();
		//Set ip and port
		serverAddress.sin_port = htons(port);
		serverAddress.sin_family = AF_INET;
		if(inet_pton(AF_INET, ip.c_str(), &serverAddress.sin_addr) < 0) {
			serverAddress.sin_family = AF_INET6;
			inet_pton(AF_INET6, ip.c_str(), &serverAddress.sin_addr);
		};

		//Setup listeners
		engine->GetEvents()->Subscribe<PreUpdateEvent>([this](const PreUpdateEvent& e) {
			//Read all new messages from server
			while(true) {
				FD_ZERO(&readfds);
				FD_SET(connection, &readfds);
				int activity = select(connection + 1, &readfds, NULL, NULL, &timeout);
	        	if (activity < 0) break; //Failed to read info

	         	if (!FD_ISSET(connection, &readfds)) break; //No new messages
				//Read id and data size
				Message message = readMessage(connection);
				if(message.id == 2) break; //Error occured, ignore message
				//Publish to listeners
				recieve(message);
			}
		});
		engine->GetEvents()->Subscribe<UpdateEvent>([this](const UpdateEvent& e) {
			sinceLastPing += e.deltaTime;

			if(sinceLastPing > TIMEOUT_PING) {
				Log::Debug("Server timed out!");
				sinceLastPing = 0;
				close(connection);
				//TODO: Currently whole program stops and waits for a new connection, make it so it just tries every update
				connect();
			}
		});
		//Ping
		listen(3, [this](auto _) {
			pingTime = sinceLastPing * 1000 + 0.5;
			//Log::Debug("Ping: " + std::to_string(pingTime));
			sinceLastPing = 0;

			send(3);
		});
		//Start connection
		if(!connect()) throw std::runtime_error("Failed to connect to server at " + ip + ":" + std::to_string(port));
	}

	bool Manager::connect() {
		//Connect to server
		connection = socket(serverAddress.sin_family, SOCK_STREAM, 0);
		if(::connect(connection, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) return false;
		//Wait for connection message
		while(true) {
			Message msg = readMessage(connection);
			if(msg.id != 0) continue;
			//Connection message recieved
			if(id == Utilities::ID::empty) {
				//First connection, save given id
				id = msg.data.Read<Utilities::ID>();
			} else {
				//Reconnection
				send(0, id);
			}
			//No more waiting
			break;
		}
		Log::Debug("Succesfully connected to server");

		send(3); //Send initial ping
		return true;
	}

	void Manager::disconnect() {
		send(1);
		::close(connection);
		id = Utilities::ID::empty;
	}

	Manager::~Manager() {
		disconnect();
	}

	void Manager::send(const Message& message) const {
		sendMessage(connection, message);
	}
	void Manager::send(const MessageID& id, MessageData data) const {
		send({id, data});
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

	MessageHandler::MessageHandler(const MessageFunction& function) : function(function) {}
	MessageHandler::MessageHandler(const MessageHandler& copy) : function(copy.function), id(copy.id) {}
	void MessageHandler::operator= (const MessageHandler& copy) {
		function = copy.function;
		id = copy.id;
	}

	void MessageHandler::operator() (MessageData message) const {
		function(message);
	}

	bool MessageHandler::operator== (const MessageHandler& other) const {
		return (other.id == id);
	}
}

#endif
