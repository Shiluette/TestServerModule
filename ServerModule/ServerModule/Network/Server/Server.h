#pragma once
#include"stdafx.h"

typedef enum SERVERSTATE {
	SERVER_STOP,
	SERVER_READY,
	SERVER_INIT,
	COUNT
};
class Server{
protected:
	char _ip[16];
	DWORD _port;
	DWORD _workerTheadCount;

	SERVERSTATE _state;
	ContentsProcess * _contentsprocess;
public:
	Server(ContentsProcess * contentsprocess);
	virtual ~Server();

	virtual void Initialize();
	virtual bool Run() = 0; // ���� �����Լ�
	SERVERSTATE & states(); 

};