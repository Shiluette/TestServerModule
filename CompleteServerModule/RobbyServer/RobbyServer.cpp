// RobbyServer.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include"serverHeader.h"


int main()
{
	IOCPServer * server = new IOCPServer(nullptr);
	server->run();
	return 0;
}

