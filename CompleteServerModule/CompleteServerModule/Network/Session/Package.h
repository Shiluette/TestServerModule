#pragma once
#include"stdafx.h"

class Session;

// ���� Ŭ���� �� ����ϸ� �ٷ� ����������.
// ����! session�� �Ժη� ����� �ȵȴ�.
class Package
{
public:
	Work * _work;
	Session * _session;
	Packet * _packet;

	Package(Session * session, Packet * packet, Work * work = nullptr)
	{
		_work = work;
		_session = session;
		_packet = packet;
	}

	
	~Package()
	{
		_work = nullptr;
		_session = nullptr;
		SAFE_DELETE(_packet);
	}
};