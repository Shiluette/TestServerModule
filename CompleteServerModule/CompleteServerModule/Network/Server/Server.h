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
	int _port;
	int _workerThreadCount;

	SERVERSTATE _state;
	ContentsProcess * _contentsprocess;
public:
	Server(ContentsProcess * contentsprocess);
	virtual ~Server();

	virtual void Initialize(jsonValue_t * config);
	virtual bool run() = 0; // 순수 가상함수
	SERVERSTATE & states(); 
	void putPackage(Package * package);

};