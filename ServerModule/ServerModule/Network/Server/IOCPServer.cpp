#include"stdafx.h"
#include "IOCPServer.h"
#include"..\Session\IOCPSession.h"

bool IOCPServer::createListenSocket()
{
	_listenSocket = WSASocket(AF_INET, SOCK_STREAM, NULL, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (_listenSocket == INVALID_SOCKET) {
		SErrLog(L" !! ListenSocket(WSASocket) failed");
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
		SErrLog(L" !! Bind Fail");
		return false;
	}
	const int BACK_LOG_SIZE = 5;
	ret = ::listen(_listenSocket, BACK_LOG_SIZE);
	if (ret == SOCKET_ERROR) {
		SErrLog(L" !! Listen Fail");
		return false;
	}
	array<char, SIZE_64> ip;
	inet_ntop(AF_INET, &(serverAdder.sin_addr), ip.data(), ip.size());

	SLog(L"** Server Listen socket created . IP : %S , Prot : %d ", ip.data(), _port);

	return true;
}

DWORD WINAPI IOCPServer::acceptThread(LPVOID serverPtr)
{
	IOCPServer * server = (IOCPServer *)serverPtr;

	while (!_shutdown) {
		SOCKET acceptSocket = INVALID_SOCKET;
		SOCKADDR_IN recvAdder;
		static int adderLen = sizeof(recvAdder);
		acceptSocket = WSAAccept(server->listenSocekt(), (struct sockaddr *)&recvAdder, &adderLen, NULL, 0);
		if (acceptSocket == SOCKET_ERROR) {
			if (!server->states() == SERVER_STOP) {
				SLog(L" !! Accpet fail");
				// ERR : DO NOT Initialize check to this function!
				break;
			}
		}
		server->onAceept(acceptSocket, recvAdder);

		if (server->states() != SERVER_READY) {
			SLog(L" !! Logic Error");
			// ERR : 거의 이 부분에 올 일이 없으나 서버의 초기화 작업이전에 
			// Accept가 이루어지면 가능성이 없지도 않음. 거의 불가능
			break;
		}
	}
	return 0;
}

DWORD WINAPI IOCPServer::workerThread(LPVOID serverPtr)
{
	IOCPServer * server = (IOCPServer *)serverPtr;

	while (!_shutdown) {
		IoData * iodata = nullptr;
		IOCPSession * session = nullptr;
		DWORD transfersize;

		BOOL retval = GetQueuedCompletionStatus(server->iocp(), &transfersize, (PULONG_PTR)&session, (LPOVERLAPPED*)&iodata, INFINITE);

		if (!retval) {
			SLog(L" ## queue data getting fail\n");
			// keepAlive 에서 소켓처리를 넘기는 방안 채택
			// Note : retval = false , transfersize = 0  : 비 정상적 종료
			//		  retval = true , transfersize = 0 : 정상적 종료
			//		  *Overlapped = null, retval = false, completionkey = null
			//		  : WAIT_TIMEOUT
			continue;
		}
		if (session == nullptr) {
			SLog(L"! socket data broken");
			return 0;
		}

		switch (iodata->type()) {
		case IO_WRITE:

			session->onSend((size_t)transfersize);
			continue;
		case IO_READ: 
		{
			Package *package = session->onRecv((size_t)transfersize);
			if (package != nullptr) {
				server->putPackage(package);
			}
		}
			continue;
		case IO_ERROR:

			SessionManager::getInstance().closeSession(session);
			continue;

		}
	}
	return 0;
}

IOCPServer::IOCPServer(ContentsProcess * contentsProcess) 
	: Server(contentsProcess)
{
}

IOCPServer::~IOCPServer()
{
	::closesocket(_listenSocket);
}

bool IOCPServer::run()
{
	if (MAXIMUM_IOCP_THREAD < _workerThreadCount) {
		SErrLog(L"! workerThread Limited[%d], this config setting Err[%d] !", MAXIMUM_IOCP_THREAD, _workerThreadCount);
		return false;
	}

	_giocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, _workerThreadCount);
	if (_giocp == nullptr) {
		SLog(L" !! _giocp err !!");
		return false;
	}
	this->createListenSocket();

	_acceptThread = MAKE_THREAD(IOCPServer, acceptThread);
	for (auto i = 0; i < _workerThreadCount; ++i) {
		_workerThread[i] = MAKE_THREAD(IOCPServer, workerThread);
	}

	this->_state = SERVER_READY;

	/*
		TODO : SessionManager 를 이용하여 람다함수 제작!
				1. All Session count 
				2. Kick Function
				3. Notify All User's
				4. Notify other's User
				5. Nomal Ping Testing
	*/
	return true;
}

SOCKET IOCPServer::listenSocekt()
{
	return _listenSocket;
}

HANDLE IOCPServer::iocp()
{
	return _giocp;
}

void IOCPServer::onAceept(SOCKET accepter, SOCKADDR_IN addressinfo)
{
	IOCPSession * session = new IOCPSession();
	if (session == nullptr) {
		SLog(L"! accpet Sesion created fail");
		return;
	}
	if (!session->onAccept(accepter, addressinfo)) {
		SAFE_DELETE(session);
		return;
	}
	if (!SessionManager::getInstance().addSession(session)) {
		SAFE_DELETE(session);
		return;
	}
	session->_ioData[IO_READ].clear();

	HANDLE handle = CreateIoCompletionPort((HANDLE)accepter, this->iocp(), (ULONG_PTR)&(*session), NULL);
	if (!handle) {
		SAFE_DELETE(session);
		return;
	}

	SLog(L" ** Client accept who's [%S]", session->clientAddress().c_str());
	session->recvStandBy();
}
