#pragma once
#include"stdafx.h"


//TODO : �ΰ��ӿ����� need ��Ŷ ����
// �濡�� �ΰ��� ����� ������ �����ؾ��t
// Ŭ���̾�Ʈ Ű �Է� (���� , ����, ���� Ű, ���� Ű) ������ ��Ŷ�� �ʿ�
// ���콺�� �̵�(ī�޶� ���� �ٶ󺸴� ������ rotate�� ������ ���� ��Ŷ�� �ʿ�)
// �C ��ġ�� ���� ��Ŷ�� �ʿ�
// �� �ɸ��� ���� ��Ŷ�� �ʿ�
// �������� ���ݹ�ư Ŭ���� ���� ��Ŷ�� �ʿ�
// ��Ʈ ������ �Ǿ����� ��Ʈ ���� �������� ������ ����(�̰� Ÿ �������Ե� ��������.. ��� �����ڿ��Դ� ������ ������ �ʿ� ���� �ʴ�)
// ������ ��ȣ�ۿ�, ������ ��ȣ�ۿ� �Ϸ�, ������ �������� ���� ������ �ʿ��ϴ�.
// ������ �����ӿ� ���� ������ �������� ����ȭ ��Ŷ�� �ʿ�

class IngameProcess: public ContentsProcess
{
public:
	IngameProcess();
	void registSubPacketFunc();

private:
	static void CPakcet_ROOMDATA(Session * session, Packet * rowPacket);		// �� �ο��� ���� ���� ����
	static void CPakcet_SYNCTIMERECV(Session *session, Packet * rowPacket);		// �ð�����ȭ
	static void CPakcet_DIRECTIVITY(Session *session, Packet * rowPacket);		// ���� ����
	static void CPacket_USERMOVE(Session *session, Packet *rowPacket);			// ���� ������
	static void CPacket_KILLERATT(Session *session, Packet *rowPacket);			// ������ ����
	static void CPacket_TRAPBUILD(Session *session, Packet * rowPacket);		// Ʈ�� ��ġ
	static void TPacket_STARTUSERDATA(Session *session, Packet * rowPacket);	// �κ񼭹����� ���� ������ ó��

	static void CPacket_SELECTPA(Session *session, Packet *rowPacket);			
	static void CPacket_SEARCHROOM(Session *session, Packet *rowPacket);		
	static void CPacket_GAMEREADYON(Session *session, Packet *rowPacket);		
	static void CPacket_GAMEREADYOFF(Session *session, Packet *rowPacket);		
	static void CPacket_EXITROOM(Session *session, Packet *rowPacket);			
	static void CPacket_GAMEOUT(Session *session, Packet *rowPacket);			
};