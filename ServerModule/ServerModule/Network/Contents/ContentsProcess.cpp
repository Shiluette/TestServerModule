#include"stdafx.h"
#include"ContentsProcess.h"

#define PROCESSCOUNT 4
void ContentsProcess::initalize()
{
	// TODO : Contents에 맞는 스레드 생성
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
		// TODO : Log 출력 필요
		// 1. 업데이트 되지 않은 타입이거나, 패킷의 조작의심
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
	// TODO : Config 클래스 제작시 Json을 이용하여... 음
	// Json을 이용하여 할 필요가 있을까나...
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
