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
public:
	Server();
	virtual ~Server();

	virtual void Initialize();
	virtual bool Run() = 0; // 순수 가상함수
	SERVERSTATE & states(); 

};