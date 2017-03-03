// RobbyServer.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include"serverHeader.h"

void serverProcess()
{
	shared_ptr<Server> server(new IOCPServer(new RobbyProcess()));
	if (!server->run()) {
		SLog(L"!!! error : server start fail");
		return;
	}
}


int main()
{
	DBManager::getInstance().run();

	shared_ptr<Thread> serverThread(new Thread(new thread_t(serverProcess), L"LobbyServer"));
	return 0;
}

