#pragma once
#include"stdafx.h"

class Session;

// ���� Ŭ���� �� ����ϸ� �ٷ� ����������.
// ����! session�� �Ժη� ����� �ȵȴ�.
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