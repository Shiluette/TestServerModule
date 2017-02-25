#pragma once
#include"stdafx.h"

// Thread count ��ȯ -> ��Ȳ������ ��������.
#define MAX_PACKET_THREAD		SIZE_64

class ContentsProcess
{
private:
	array<Thread *, MAX_PACKET_THREAD> _threadPool;
	ThreadJobQueue<Package *> * _packageQueue;

protected:
	typedef void(*Runfunc)(Session * session, Packet * rowPacket);
	typedef void(*Runwork)(Work * rowWork);
	std::unordered_map<PacketType, Runfunc> _runFuncTable;

private:
	void initalize(jsonValue_t * config);
	void registDefaultPacketFunc();
	void run(Package * package);
	void execute();

	void process();
public:
	ContentsProcess();
	virtual ~ContentsProcess();

	void putPackage(Package *package);

	virtual void registSubPacketFunc() = 0; // ���� ������ ��ӿ� �Լ�
	

};