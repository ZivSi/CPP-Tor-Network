#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h> 
#include <map>
#include <string>
#include <vector>
#include "RSA.hpp"
#include "Constants.h"
#include "ClientData.hpp"
#include "NodeData.hpp"

#pragma comment(lib, "ws2_32.lib")

using std::cout;
using std::endl;
using std::map;
using std::string;
using std::vector;
using std::to_string;


class ServerObject {
public:
	ServerObject();
	~ServerObject();

	void startServer(int port);
	void stopServer();


private:
	SOCKET _socket;
	SOCKADDR_IN _addr;

	RSA rsaObject;

	string rsaKeysFormat = to_string(rsaObject.getPublicKey()) + SPLITER + to_string(rsaObject.getModulus());

	map<int, ClientData> clientsMap = map<int, ClientData>();
	map<int, NodeData> nodesMap = map<int, NodeData>();

	SOCKET initWSA();

	bool bindSocket(SOCKET serverSocket, int port);
	bool listenSocket(SOCKET serverSocket);

	SOCKET acceptClient(SOCKET serverSocket, sockaddr_in* clientAddress, int* clientAddressSize);
	bool clientNotAvaiable(SOCKET clientSocket);

	vector<int> generateRandomPath(int length);

	string encryptMessage(string message, vector<int> generatedPath);

	void sendToFirstNode(string message, int firstNodeServerPort);

	void terminateNode(int nodeServerPort);

	void sendRSAKeys(SOCKET clientSocket);
};