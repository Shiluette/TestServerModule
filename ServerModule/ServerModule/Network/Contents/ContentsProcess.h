#pragma once
#include"stdafx.h"

// Thread count 변환 -> 상황에따라 수정가능.
#define MAX_PACKET_THREAD		SIZE_64

class ContentsProcess
{
private:
	array<Thread *, MAX_PACKET_THREAD> _threadPool;
	// ThreadJobQueue<Package> *_packageQueue;

protected:
	// typedef void(*Runfunc)(Session * session, Packet * rowPacket);
};