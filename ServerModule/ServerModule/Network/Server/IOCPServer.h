#pragma once
#include"stdafx.h"
#include"Server.h"
//Iocp ���� Ŭ����.

class IOCPServer : public Server
{
	SOCKET _listenSocket;
	HANDLE _giocp;
	Thread * _acceptThread;
	array<Thread *, unsigned DWORD>;
public:
	IOCPServer();
};
