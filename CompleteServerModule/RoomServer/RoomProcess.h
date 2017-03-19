#pragma once
#include"stdafx.h"


//TODO : 인게임에서의 need 패킷 종류
// 방에서 인게임 입장시 정보를 전달해야햠
// 클라이언트 키 입력 (전진 , 후진, 좌측 키, 우측 키) 에대한 패킷이 필요
// 마우스의 이동(카메라에 따른 바라보는 방향의 rotate의 변동에 대한 패킷이 필요)
// 덪 설치에 따른 패킷이 필요
// 덫 걸림에 따른 패킷이 필요
// 살인자의 공격버튼 클릭에 따른 패킷이 필요
// 히트 판정이 되었을시 히트 맞은 유저에게 정보를 제공(이건 타 유저에게도 마찬가지.. 대신 살인자에게는 정보의 제공이 필요 하지 않다)
// 발전기 상호작용, 발전기 상호작용 완료, 발전기 게이지에 대한 정보가 필요하다.
// 유저의 움직임에 따른 서버의 예측계산과 동기화 패킷이 필요

class IngameProcess: public ContentsProcess
{
public:
	IngameProcess();
	void registSubPacketFunc();

private:
	static void CPakcet_ROOMDATA(Session * session, Packet * rowPacket);		// 방 인원에 대한 정보 수신
	static void CPakcet_SYNCTIMERECV(Session *session, Packet * rowPacket);		// 시간동기화
	static void CPakcet_DIRECTIVITY(Session *session, Packet * rowPacket);		// 방향 변경
	static void CPacket_USERMOVE(Session *session, Packet *rowPacket);			// 유저 움직임
	static void CPacket_KILLERATT(Session *session, Packet *rowPacket);			// 살인자 공격
	static void CPacket_TRAPBUILD(Session *session, Packet * rowPacket);		// 트랩 설치
	static void TPacket_STARTUSERDATA(Session *session, Packet * rowPacket);	// 로비서버에서 받은 데이터 처리

	static void CPacket_SELECTPA(Session *session, Packet *rowPacket);			
	static void CPacket_SEARCHROOM(Session *session, Packet *rowPacket);		
	static void CPacket_GAMEREADYON(Session *session, Packet *rowPacket);		
	static void CPacket_GAMEREADYOFF(Session *session, Packet *rowPacket);		
	static void CPacket_EXITROOM(Session *session, Packet *rowPacket);			
	static void CPacket_GAMEOUT(Session *session, Packet *rowPacket);			
};