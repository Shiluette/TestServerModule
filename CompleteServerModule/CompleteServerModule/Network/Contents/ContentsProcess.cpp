#include"stdafx.h"
#include"ContentsProcess.h"

void ContentsProcess::initalize(jsonValue_t * config)
{
	jsonValue_t contents = (*config)["Contents"];
	if (contents == NULL) {
		SErrLog(L"** not exist process setting **");
		return;
	}
	int processCount = contents.get("ThreadCount", 0).asInt();
	if (processCount == 0) {
		SLog(L" ** not to do this Thread create, input zero count **");
		return;
	}


	_packageQueue = new ThreadJobQueue<Package *>(L"ContentsProcessQueue");
	for (auto i = 0; i < processCount; ++i) {
		_threadPool[i] = MAKE_THREAD(ContentsProcess, process);
	}

	this->registDefaultPacketFunc();
}

void ContentsProcess::registDefaultPacketFunc()
{

}

void ContentsProcess::run(Package * package)
{
	if (package->_work != nullptr) {
		package->_work->tick();
		return;
	}
	PacketType type = package->_packet->type();
	Runfunc runFunction = _runFuncTable.at(type);
	if (runFunction == nullptr) {
		SLog(L"!! invalid Packet inputted... type[%d]", type);
		package->_session->onClose();
		return;
		// 1. 업데이트 되지 않은 타입이거나, 패킷의 조작의심
	}
	SLog(L"*** [%d] packet run ***", type);
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
	jsonValue_t config;
	if (!loadConfig(&config)) {
		return;
	}
	this->initalize(&config);
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
