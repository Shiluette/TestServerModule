#pragma once
#include"stdafx.h"

class RobbyProcess : public ContentsProcess
{
public:
	RobbyProcess();
	virtual ~RobbyProcess();
	void registSubPacketFunc();
	
private:
	static void CPacket_LOGIN(Session *session, Packet *rowPacket);			// 로그인 요청
	static void CPacket_JOIN(Session *session, Packet *rowPacket);			// 회원 가입 요청
	static void CPacket_SELECTPART(Session *session, Packet *rowPacket);	// 역할 설정
	static void CPacket_SEARCHROOM(Session *session, Packet *rowPacket);	// 방 찾기
	static void CPacket_GAMEREADYON(Session *session, Packet *rowPacket);	// 준비상태 ON
	static void CPacket_GAMEREADYOFF(Session *session, Packet *rowPacket);	// 준비 상태 OFF
	static void CPacket_EXITROOM(Session *session, Packet *rowPacket);		// 방 나가기
	static void CPacket_GAMEOUT(Session *session, Packet *rowPacket);		// 게임 나가기
};