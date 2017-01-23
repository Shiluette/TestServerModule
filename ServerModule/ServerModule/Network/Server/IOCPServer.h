#pragma once
#include"stdafx.h"
#include"Server.h"
//Iocp 서버 클래스.

class IOCPServer : public Server
{
	SOCKET _listenSocket;
	HANDLE _giocp;
	Thread * _acceptThread;
	array<Thread *, unsigned DWORD>;
public:
	IOCPServer();
};
