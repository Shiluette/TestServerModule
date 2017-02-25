#pragma once
#include "stdafx.h" 
#include"packetHeader.h"
#include"Packet.h"




class PacketFactory : public Singleton<PacketFactory>
{
public:
	Packet* getPacket(Int64 packetType) {
		switch(packetType) {
			case PE_C_REQ_ATT:		return new PK_C_REQ_ATT();
			case PE_C_REQ_EXITROOM:		return new PK_C_REQ_EXITROOM();
			case PE_C_REQ_GAMEOUT:		return new PK_C_REQ_GAMEOUT();
			case PE_C_REQ_GAMEREADYOFF:		return new PK_C_REQ_GAMEREADYOFF();
			case PE_C_REQ_GAMEREADYON:		return new PK_C_REQ_GAMEREADYON();
			case PE_C_REQ_GENEOFF:		return new PK_C_REQ_GENEOFF();
			case PE_C_REQ_GENEON:		return new PK_C_REQ_GENEON();
			case PE_C_REQ_JOIN:		return new PK_C_REQ_JOIN();
			case PE_C_REQ_LOGIN:		return new PK_C_REQ_LOGIN();
			case PE_C_REQ_OUTSTAGE:		return new PK_C_REQ_OUTSTAGE();
			case PE_C_REQ_OWNLOOK:		return new PK_C_REQ_OWNLOOK();
			case PE_C_REQ_OWNMOVE:		return new PK_C_REQ_OWNMOVE();
			case PE_C_REQ_SEARCHROOM:		return new PK_C_REQ_SEARCHROOM();
			case PE_C_REQ_SELECTPART:		return new PK_C_REQ_SELECTPART();
			case PE_C_REQ_SITDOWN:		return new PK_C_REQ_SITDOWN();
			case PE_C_REQ_STANDUP:		return new PK_C_REQ_STANDUP();
			case PE_C_REQ_TRAPON:		return new PK_C_REQ_TRAPON();
			case PE_C_REQ_WALKOFF:		return new PK_C_REQ_WALKOFF();
			case PE_C_REQ_WALKON:		return new PK_C_REQ_WALKON();
			case PE_S_ANS_ADDUSERVIEW:		return new PK_S_ANS_ADDUSERVIEW();
			case PE_S_ANS_ALIVEUSERSTATEUPDATE:		return new PK_S_ANS_ALIVEUSERSTATEUPDATE();
			case PE_S_ANS_DEFEATSTATE:		return new PK_S_ANS_DEFEATSTATE();
			case PE_S_ANS_ENTERROOMFAIL:		return new PK_S_ANS_ENTERROOMFAIL();
			case PE_S_ANS_ENTERROOMSUCC:		return new PK_S_ANS_ENTERROOMSUCC();
			case PE_S_ANS_GENONSUCC:		return new PK_S_ANS_GENONSUCC();
			case PE_S_ANS_GENSTATEUPDATE:		return new PK_S_ANS_GENSTATEUPDATE();
			case PE_S_ANS_HITJUDGE:		return new PK_S_ANS_HITJUDGE();
			case PE_S_ANS_JOINFAIL:		return new PK_S_ANS_JOINFAIL();
			case PE_S_ANS_JOINSUCC:		return new PK_S_ANS_JOINSUCC();
			case PE_S_ANS_LOGINFAIL:		return new PK_S_ANS_LOGINFAIL();
			case PE_S_ANS_LOGINSUCC:		return new PK_S_ANS_LOGINSUCC();
			case PE_S_ANS_OUTROOMSUCC:		return new PK_S_ANS_OUTROOMSUCC();
			case PE_S_ANS_SUBUSERVIEW:		return new PK_S_ANS_SUBUSERVIEW();
			case PE_S_ANS_TRAPBUILDFAIL:		return new PK_S_ANS_TRAPBUILDFAIL();
			case PE_S_ANS_TRAPBUILDSUCC:		return new PK_S_ANS_TRAPBUILDSUCC();
			case PE_S_ANS_TRAPEXECUTE:		return new PK_S_ANS_TRAPEXECUTE();
			case PE_S_ANS_USERMOVE:		return new PK_S_ANS_USERMOVE();
			case PE_S_ANS_WINSTATE:		return new PK_S_ANS_WINSTATE();
			case PE_S_BRD_GAMESTARTSUCC:		return new PK_S_BRD_GAMESTARTSUCC();
			case PE_S_BRD_OUTGAMESTATE:		return new PK_S_BRD_OUTGAMESTATE();
			case PE_S_BRD_ROOMSTATE:		return new PK_S_BRD_ROOMSTATE();
			case PE_S_NTF_OVERLAPCRUSH:		return new PK_S_NTF_OVERLAPCRUSH();
		}
		return nullptr;
	}
};
