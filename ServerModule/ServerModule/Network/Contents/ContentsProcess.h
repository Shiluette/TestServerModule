#pragma once
#include"stdafx.h"

// Thread count ��ȯ -> ��Ȳ������ ��������.
#define MAX_PACKET_THREAD		SIZE_64

class ContentsProcess
{
private:
	array<Thread *, MAX_PACKET_THREAD> _threadPool;
	// ThreadJobQueue<Package> *_packageQueue;

protected:
	// typedef void(*Runfunc)(Session * session, Packet * rowPacket);
};