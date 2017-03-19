#pragma once
#include "stdafx.h" 
#include"packetHeader.h"
#include"Packet.h"




class PacketFactory : public Singleton<PacketFactory>
{
public:
	Packet* getPacket(Int64 packetType) {
		switch(packetType) {
			case PE_C_REQ_ATT:		return new PK_C_REQ_ATT();														// 공격 요청
			case PE_C_REQ_EXITROOM:		return new PK_C_REQ_EXITROOM();												// 방 나가기
			case PE_C_REQ_GAMEOUT:		return new PK_C_REQ_GAMEOUT();												// 게임 나가기
			case PE_C_REQ_GAMEREADYOFF:		return new PK_C_REQ_GAMEREADYOFF();										// 준비 풀기
			case PE_C_REQ_GAMEREADYON:		return new PK_C_REQ_GAMEREADYON();										// 준비 완료
			case PE_C_REQ_GENEOFF:		return new PK_C_REQ_GENEOFF();												// 발전기 상호작용 off
			case PE_C_REQ_GENEON:		return new PK_C_REQ_GENEON();												// 발전기 상호작용 on
			case PE_C_REQ_JOIN:		return new PK_C_REQ_JOIN();														// 가입하기
			case PE_C_REQ_LOGIN:		return new PK_C_REQ_LOGIN();												// 로그인 요청
			case PE_C_REQ_OUTSTAGE:		return new PK_C_REQ_OUTSTAGE();												// 스테이지 나가기
			case PE_C_REQ_OWNLOOK:		return new PK_C_REQ_OWNLOOK();												// 내가 바라보는 방향
			case PE_C_REQ_OWNMOVE:		return new PK_C_REQ_OWNMOVE();												// 나의 움직임
			case PE_C_REQ_SEARCHROOM:		return new PK_C_REQ_SEARCHROOM();										// 방 찾기
			case PE_C_REQ_SELECTPART:		return new PK_C_REQ_SELECTPART();										// 역할 선택
			case PE_C_REQ_SITDOWN:		return new PK_C_REQ_SITDOWN();												// 앉기
			case PE_C_REQ_STANDUP:		return new PK_C_REQ_STANDUP();												// 일어서기
			case PE_C_REQ_TRAPON:		return new PK_C_REQ_TRAPON();												// 함정 설치
			case PE_C_REQ_WALKOFF:		return new PK_C_REQ_WALKOFF();												// 걷기 비활성화
			case PE_C_REQ_WALKON:		return new PK_C_REQ_WALKON();												// 걷기 활성화
			case PE_S_ANS_ADDUSERVIEW:		return new PK_S_ANS_ADDUSERVIEW();										// 생존자 추가
			case PE_S_ANS_ALIVEUSERSTATEUPDATE:		return new PK_S_ANS_ALIVEUSERSTATEUPDATE();						// 생존자 생태 업데이트
			case PE_S_ANS_DEFEATSTATE:		return new PK_S_ANS_DEFEATSTATE();										// 패배 상태
			case PE_S_ANS_ENTERROOMFAIL:		return new PK_S_ANS_ENTERROOMFAIL();								// 방 입장 성공
			case PE_S_ANS_ENTERROOMSUCC:		return new PK_S_ANS_ENTERROOMSUCC();								// 방 입장 실패
			case PE_S_ANS_GENONSUCC:		return new PK_S_ANS_GENONSUCC();										// 발전기 가동 성공
			case PE_S_ANS_GENSTATEUPDATE:		return new PK_S_ANS_GENSTATEUPDATE();								// 발전기 상태 업데이트
			case PE_S_ANS_HITJUDGE:		return new PK_S_ANS_HITJUDGE();												// 맞기 성공
			case PE_S_ANS_JOINFAIL:		return new PK_S_ANS_JOINFAIL();												// 회원가입 실패
			case PE_S_ANS_JOINSUCC:		return new PK_S_ANS_JOINSUCC();												// 회원가입 성공
			case PE_S_ANS_LOGINFAIL:		return new PK_S_ANS_LOGINFAIL();										// 로그인 실패
			case PE_S_ANS_LOGINSUCC:		return new PK_S_ANS_LOGINSUCC();										// 로그인 성공
			case PE_S_ANS_OUTROOMSUCC:		return new PK_S_ANS_OUTROOMSUCC();										// 방 나가기 성공
			case PE_S_ANS_SUBUSERVIEW:		return new PK_S_ANS_SUBUSERVIEW();										// 
			case PE_S_ANS_TRAPBUILDFAIL:		return new PK_S_ANS_TRAPBUILDFAIL();								// 트랩 설치 실패
			case PE_S_ANS_TRAPBUILDSUCC:		return new PK_S_ANS_TRAPBUILDSUCC();								// 트랩 설치 성공
			case PE_S_ANS_TRAPEXECUTE:		return new PK_S_ANS_TRAPEXECUTE();										// 트랩 활성화
			case PE_S_ANS_USERMOVE:		return new PK_S_ANS_USERMOVE();												// 유저 움직임 업데이트
			case PE_S_ANS_WINSTATE:		return new PK_S_ANS_WINSTATE();												// 승리 상태
			case PE_S_BRD_GAMESTARTSUCC:		return new PK_S_BRD_GAMESTARTSUCC();								// 게임 시작 성공
			case PE_S_BRD_OUTGAMESTATE:		return new PK_S_BRD_OUTGAMESTATE();										// 방 나가기 상태 브로드캐스트
			case PE_S_BRD_ROOMSTATE:		return new PK_S_BRD_ROOMSTATE();										// 방상태 업데이트
			case PE_S_NTF_OVERLAPCRUSH:		return new PK_S_NTF_OVERLAPCRUSH();										// 중복뭐시기?
			case PE_S_ANS_SELECTPARTSUCC:	return new PK_S_ANS_SELECTPARTSUCC();									// 역할 지정 성공
			case PE_S_NTF_ROOMSEARCHNOTROLE: return new PK_S_NTF_ROOMSEARCHNOTROLE();								// 방찾기 실패 - 역할 미설정
			case PE_T_NTF_STARTUSERDATA : return new PK_T_NTF_STARTUSERDATA();
		}
		return nullptr;
	}
};
