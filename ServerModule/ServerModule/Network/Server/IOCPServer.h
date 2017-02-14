#pragma once
#include"stdafx.h"
#include"Server.h"
//Iocp 서버 클래스.

#define MAXIMUM_IOCP_THREAD		SIZE_32

class IOCPServer : public Server, public Singleton<IOCPServer>
{
	SOCKET _listenSocket;
	HANDLE _giocp;
	Thread * _acceptThread;
	array<Thread *, SIZE_32> _workerThread;
private:
	bool createListenSocket();

	static DWORD WINAPI acceptThread(LPVOID serverPtr);
	static DWORD WINAPI workerThread(LPVOID serverPtr);
public:
	IOCPServer(ContentsProcess * contentsProcess);
	virtual ~IOCPServer();
	bool run();

	SOCKET listenSocekt();
	HANDLE iocp();

	void onAceept(SOCKET accepter, SOCKADDR_IN addressinfo);
};
