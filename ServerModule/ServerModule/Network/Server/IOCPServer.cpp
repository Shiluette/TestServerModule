#include"stdafx.h"
#include "IOCPServer.h"

bool IOCPServer::createListenSocket()
{
	_listenSocket = WSASocket(AF_INET, SOCK_STREAM, NULL, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (_listenSocket == INVALID_SOCKET) {
		// TODO : ADD TO LOG
		return false;
	}

	SOCKADDR_IN serverAdder;
	serverAdder.sin_family = AF_INET;
	serverAdder.sin_port = htons((u_short)_port);
	inet_pton(AF_INET, _ip, &(serverAdder.sin_addr));

	int bf = 1;
	setsockopt(_listenSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&bf, (int)sizeof(bf));

	int ret = ::bind(_listenSocket, (SOCKADDR *)&serverAdder, sizeof(serverAdder));

	if (ret == SOCKET_ERROR) {
		// TODO : ADD TO LOG
		return false;
	}
	const int BACK_LOG_SIZE = 5;
	ret = ::listen(_listenSocket, BACK_LOG_SIZE);
	if (ret == SOCKET_ERROR) {
		// TODO : ADD TO LOG
		return false;
	}

	return true;
}

DWORD IOCPServer::acceptThread(LPVOID serverPtr)
{
	IOCPServer * server = (IOCPServer *)serverPtr;

	while (!_shutdown) {
		SOCKET acceptSocket = INVALID_SOCKET;
		SOCKADDR_IN recvAdder;
		static int adderLen = sizeof(recvAdder);
		acceptSocket = WSAAccept(server->listenSocekt(), (struct sockaddr *)&recvAdder, &adderLen, NULL, 0);
		if (acceptSocket == SOCKET_ERROR) {
			if (!server->states() == SERVER_STOP) {
				// TODO : ADD TO LOG!
				// ERR : DO NOT Initialize check to this function!
				break;
			}
		}
		server->onAceept(acceptSocket, recvAdder);

		if (server->states() != SERVER_READY) {
			// ERR : 거의 이 부분에 올 일이 없으나 서버의 초기화 작업이전에 
			// Accept가 이루어지면 가능성이 없지도 않음. 거의 불가능
			break;
		}
	}
	return 0;
}

DWORD IOCPServer::workerThread(LPVOID serverPtr)
{
	return 0;
}

IOCPServer::IOCPServer(ContentsProcess * contentsProcess) : Server(contentsProcess)
{
}

IOCPServer::~IOCPServer()
{
	::closesocket(_listenSocket);
}

bool IOCPServer::run()
{
	return false;
}

SOCKET IOCPServer::listenSocekt()
{
	return SOCKET();
}

HANDLE IOCPServer::iocp()
{
	return HANDLE();
}

void IOCPServer::onAceept(SOCKET accepter, SOCKADDR_IN addrInfo)
{
	// TODO : 아직 미진행.
}
