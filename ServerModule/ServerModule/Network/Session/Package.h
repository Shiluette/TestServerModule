#pragma once
#include"stdafx.h"

class Session;

// 소포 클래스 뜯어서 사용하면 바로 지워버리자.
// 주의! session은 함부로 지우면 안된다.
class Package
{
public:
	Session * _session;
	Packet * _packet;

	Package(Session * session, Packet * packet)
	{
		_session = session;
		_packet = packet;
	}

	
	~Package()
	{
		_session = nullptr;
		SAFE_DELETE(_packet);
	}
};