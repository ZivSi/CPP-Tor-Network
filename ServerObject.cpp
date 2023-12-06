#include "ServerObject.h"

ServerObject::ServerObject() {
	startServer(5060);
}

ServerObject::~ServerObject() {
	// stopServer();

	WSACleanup();
}

SOCKET ServerObject::initWSA() {
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "WSAStartup failed." << std::endl;

		WSACleanup();

		exit(1);
	}

	// Create a socket
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (serverSocket == INVALID_SOCKET) {
		std::cerr << "Socket creation failed." << std::endl;

		WSACleanup();

		exit(1);
	}


	return serverSocket;
}

bool ServerObject::bindSocket(SOCKET serverSocket, int port) {
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(static_cast<u_short>(port));

	if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		std::cerr << "Socket binding failed." << std::endl;
		return false;
	}

	return true;
}

bool ServerObject::listenSocket(SOCKET serverSocket) {
	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cerr << "Socket listening failed." << std::endl;
		return false;
	}

	return true;
}

void ServerObject::startServer(int port) {
	SOCKET serverSocket = initWSA();

	bool bindSuccessful = bindSocket(serverSocket, port);

	if (!bindSuccessful) {
		closesocket(serverSocket);
		WSACleanup();
		exit(1);
	}

	bool listenSuccessful = listenSocket(serverSocket);

	if (!listenSuccessful) {
		closesocket(serverSocket);
		WSACleanup();
		exit(2);
	}

	cout << "Server is listening on " << port << endl;


	// Start accepting clients
	while (true) {
		SOCKET clientSocket;
		sockaddr_in clientAddress;
		int clientAddressSize = sizeof(clientAddress);

		clientSocket = acceptClient(serverSocket, &clientAddress, &clientAddressSize);

		if (clientNotAvaiable(clientSocket)) {
			std::cerr << "Accept failed" << std::endl;

			continue;
		}

		char clientIP[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(clientAddress.sin_addr), clientIP, INET_ADDRSTRLEN);

		std::cout << "Connection accepted from " << clientIP << ":" << ntohs(clientAddress.sin_port) << std::endl;




		// Handle Client
	}
}

SOCKET ServerObject::acceptClient(SOCKET serverSocket, sockaddr_in* clientAddress, int* clientAddressSize) {
	return accept(serverSocket, (struct sockaddr*)clientAddress, clientAddressSize);
}

bool ServerObject::clientNotAvaiable(SOCKET clientSocket) {
	return clientSocket == INVALID_SOCKET;
}