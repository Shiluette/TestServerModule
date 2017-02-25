#pragma once
#include"stdafx.h"

class RobbyProcess : public ContentsProcess
{
public:
	RobbyProcess();
	virtual ~RobbyProcess();
	void registSubPacketFunc();
	
private:
	static void CPacket_LOGIN(Session *session, Packet *rowPacket);			// �α��� ��û
	static void CPacket_JOIN(Session *session, Packet *rowPacket);			// ȸ�� ���� ��û
	static void CPacket_SELECTPART(Session *session, Packet *rowPacket);	// ���� ����
	static void CPacket_SEARCHROOM(Session *session, Packet *rowPacket);	// �� ã��
	static void CPacket_GAMEREADYON(Session *session, Packet *rowPacket);	// �غ���� ON
	static void CPacket_GAMEREADYOFF(Session *session, Packet *rowPacket);	// �غ� ���� OFF
	static void CPacket_EXITROOM(Session *session, Packet *rowPacket);		// �� ������
	static void CPacket_GAMEOUT(Session *session, Packet *rowPacket);		// ���� ������
};