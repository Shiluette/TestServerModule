// RoomServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include"serverHeader.h"


void serverProcess()
{
	shared_ptr<Server> server(new IOCPServer(new IngameProcess()));
	if (!server->run()) {
		SLog(L"!!! error : server start fail");
		return;
	}
}


int main()
{
	DBManager::getInstance().run();
	UserManager::getInstance();
	shared_ptr<Thread> serverThread(new Thread(new thread_t(serverProcess), L"RoomServer"));
	return 0;
}

