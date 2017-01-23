#include"stdafx.h"
#include"Server.h"

Server::Server()
{
	_state = SERVER_STOP;
}

Server::~Server()
{
	_state = SERVER_STOP;
}

void Server::Initialize()
{
	_state = SERVER_INIT;
}

SERVERSTATE & Server::states()
{
	return _state;
	// TODO: 여기에 반환 구문을 삽입합니다.
}
