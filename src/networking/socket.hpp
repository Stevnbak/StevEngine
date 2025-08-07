#pragma once
#ifdef StevEngine_NETWORKING
#ifdef WINDOWS
	#include <winsock2.h>
	#pragma comment(lib, "ws2_32.lib") //Winsock Library
	WSADATA wsa;
	SOCKET sockfd;
	SOCKET connected_socket;
#else
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <unistd.h>

	int sockfd = -1;
	int connected_socket = -1;
#endif

struct sockaddr_in address;

int socketConnect();

int initSocket() {
	printf("Initializing socket...\n");
	#ifdef WINDOWS
		printf("\nInitialising Winsock...\n");
		if (WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		{
			printerror("winsock");
			return 1;
		}
	#endif
	//Create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
        printerror("socket failed");
        return -1;
    }
	//Bind socket to port 5000
	/*int opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
		perror("setsockopt");
		return -1;
	}//*/
	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; //localhost
    address.sin_port = htons(5000);
	if(bind(sockfd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		printerror("bind failed");
        return -1;
	}
	//Listen for connections
	if (listen(sockfd, 1) < 0) {
        printerror("listen");
        return -1;
    }
	//Accept a single connection
	return socketConnect();
}

int socketConnect() {
	printf("Waiting for socket connection...\n");
	#ifdef WINDOWS
		//auto addrlen = sizeof(address);
		connected_socket = accept(sockfd, NULL, NULL);
	#else
		socklen_t addrlen = sizeof(address);
		connected_socket = accept(sockfd, (struct sockaddr*)&address, &addrlen);
	#endif
	if (connected_socket < 0) {
		printerror("accept failed");
		return -1;
	}
	printf("Socket connection established!\n");
	return 0;
}

void sendMessage(char* msg) {
	printf("Sending message...\n");
	send(connected_socket, msg, strlen(msg), 0);
	printf("Message sent succesfully!\n");
}

void getMessage(char* msg, int bufferLen) {
	printf("Waiting for message...\n");
	#ifdef WINDOWS
		int count = recv(connected_socket, msg, bufferLen - 1, 0);
	#else
		int count = read(connected_socket, msg, bufferLen - 1);
	#endif
	if (count < 0) {
		socketConnect();
		getMessage(msg, bufferLen);
	} else {
		msg[count] = '\0';
		printf("Message: %s\n", msg);
	}
}

void sendDeck() {
	char deckStr[200];
	convertDeckToStr(deck, deckStr);
	char out[400];
	sprintf(out, "deck|%s|%s|%s", deckStr, getStatusMessage(), getLastCommand());
	sendMessage(out);
}

void sendGame() {
	char gameStr[200];
	convertGameToStr(&game, gameStr);
	char out[400];
	sprintf(out, "game|%s|%s|%s", gameStr, getStatusMessage(), getLastCommand());
	sendMessage(out);
}
#endif