#include"stdafx.h"
#include"ContentsProcess.h"

#define PROCESSCOUNT 4
void ContentsProcess::initalize()
{
	// TODO : Contents�� �´� ������ ����
	_packageQueue = new ThreadJobQueue<Package *>(L"ContentsProcessQueue");
	for (auto i = 0; i < PROCESSCOUNT; ++i) {
		_threadPool[i] = MAKE_THREAD(ContentsProcess, process);
	}

	this->registDefaultPacketFunc();
}

void ContentsProcess::registDefaultPacketFunc()
{

}

void ContentsProcess::run(Package * package)
{
	PacketType type = package->_packet->type();
	Runfunc runFunction = _runFuncTable.at(type);
	if (runFunction == nullptr) {
		// TODO : Log ��� �ʿ�
		// 1. ������Ʈ ���� ���� Ÿ���̰ų�, ��Ŷ�� �����ǽ�
	}
	runFunction(package->_session, package->_packet);
}

void ContentsProcess::execute()
{
	Package *package = nullptr;
	if (_packageQueue->pop(package) == false) {
		return;
	}
	this->run(package);
	SAFE_DELETE(package);
}

void ContentsProcess::process()
{
	while (_shutdown == false) {
		this->execute();
		CONTEXT_SWITCH;
	}
}

ContentsProcess::ContentsProcess()
{
	// TODO : Config Ŭ���� ���۽� Json�� �̿��Ͽ�... ��
	// Json�� �̿��Ͽ� �� �ʿ䰡 �����...
	this->initalize();
}

ContentsProcess::~ContentsProcess()
{
	SAFE_DELETE(_packageQueue);
	for(auto thread : _threadPool) {
		SAFE_DELETE(thread);
	}
	_runFuncTable.clear();
}

void ContentsProcess::putPackage(Package * package)
{
	_packageQueue->push(package);
}
