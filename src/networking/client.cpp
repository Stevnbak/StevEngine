#ifdef StevEngine_NETWORKING
#include "client.hpp"
#include "networking.hpp"
#include "main/Engine.hpp"
#include "main/EngineEvents.hpp"
#include "main/Log.hpp"
#include "utilities/Stream.hpp"

#include <cstdint>
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
		//Establish connection
		connection = socket(serverAddress.sin_family, SOCK_STREAM, 0);
		if(!connect()) throw std::runtime_error("Failed to connect to server at " + ip + ":" + std::to_string(port));
		FD_ZERO(&readfds);
		FD_SET(connection, &readfds);

		//Wait for connection confirmation
		MessageID id;
		if(recv(connection, &id, sizeof(id), 0) < sizeof(id)) throw std::runtime_error("Error when reading messages from server."); //TODO: Handle partial messages better
		if(id != 0) throw std::runtime_error("Invalid first message from server.");
		Log::Debug("Succesfully connected to server");
		uint32_t size;
		recv(connection, &size, sizeof(size), 0); //Remove size info from socket buffer

		engine->GetEvents()->Subscribe<PreUpdateEvent>([this](const PreUpdateEvent& e) {
			//Read all new messages from server
			while(true) {
				int activity = select(connection + 1, &readfds, NULL, NULL, &timeout);
	        	if (activity < 0) break; //Failed to read info

	         	if (!FD_ISSET(connection, &readfds)) break; //No new messages
				//Read id and data size
				MessageID id;
				uint32_t size;
				if(recv(connection, &id, sizeof(id), 0) < sizeof(id)) break; //TODO: Handle partial messages better
				if(recv(connection, &size, sizeof(size), 0) < sizeof(id)) break;
				//Read data
				Utilities::Stream stream(Utilities::Binary);
				if(size > 0) {
					char* buf = new char[size];
					if(recv(connection, buf, size, 0) < size) {
						delete[] buf;
						break;
					}
					for(uint32_t i = 0; i < size; i++) stream << buf[i];
					delete[] buf;
				}
				//Publish to listeners
				recieve({id, stream});
			}
		});
	}

	bool Manager::connect() {
		//Connect to this server
		return ::connect(connection, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) >= 0;
	}

	void Manager::disconnect() {
		send(1);
		::close(connection);
	}

	Manager::~Manager() {
		disconnect();
	}

	bool Manager::send(const Message& message) {
		//Create data
		Utilities::Stream raw(Utilities::Binary);
		raw << message.id << message.data.GetStream().view().size() << message.data;
		//Try and send data
		int tries = 0;
		while(::send(connection, raw.GetStream().view().data(), raw.GetStream().view().size(), 0) < 0) {
			if(tries > 3) return false;
			//Try to establish connection again
			close(connection);
			if(!connect()) return false;
			tries++;
		}
		//Message sent succesfully
		return true;
	}

	void Manager::recieve(const Message& message) {
		auto& handlers = subscribers[message.id];
		auto i = handlers.begin();
		while(i != handlers.end()) {
			(*i)(message);
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

	void MessageHandler::operator() (Message message) const {
		function(message);
	}

	bool MessageHandler::operator== (const MessageHandler& other) const {
		return (other.id == id);
	}
}

#endif
