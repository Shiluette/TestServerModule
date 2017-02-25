#include"stdafx.h"
#include"Server.h"

Server::Server(ContentsProcess * contentsprocess)
{
	//SLog(L" ## init network ������ �غ�");
	_contentsprocess = contentsprocess;
	_state = SERVER_STOP;
	jsonValue_t config;
	if (!loadConfig(&config)) {
		return;
	}
	this->Initialize(&config);
}

Server::~Server()
{
	shutdownServer();
	_state = SERVER_STOP;
	SAFE_DELETE(_contentsprocess);
	SLog(L"$$ End network Base");
}

void Server::Initialize(jsonValue_t * config)
{
	// �׽�ũ ����
	TaskManager::getInstance();
	
	// ���� ����
	
	jsonValue_t Vserver = (*config)["Server"];
	strcpy_s(_ip, Vserver.get("IP", "").asString().c_str());
	_port = Vserver.get("PORT", 0).asInt();
	_workerThreadCount = Vserver.get("ThreadCount", 0).asInt();
	_state = SERVER_INIT;
	SLog(L"** worker thread count : %d", _workerThreadCount);
	string _name = (*config).get("Name", "").asString();
	SLog(L"### %S Server Start~! ###", _name.data());
}

SERVERSTATE & Server::states()
{
	return _state;
}

void Server::putPackage(Package * package)
{
	_contentsprocess->putPackage(package);
}
