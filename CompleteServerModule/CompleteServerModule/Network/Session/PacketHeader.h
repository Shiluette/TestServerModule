#pragma once
#include"stdafx.h"


#define PACKET_MAKE_DATA "{0}"
enum PacketType : Int64 {
	/* 1000 */		PE_C_REQ_ATT = 1000 ,
	/* 1001 */		PE_C_REQ_EXITROOM = 1001 ,
	/* 1002 */		PE_C_REQ_GAMEOUT = 1002 ,
	/* 1003 */		PE_C_REQ_GAMEREADYOFF = 1003 ,
	/* 1004 */		PE_C_REQ_GAMEREADYON = 1004 ,
	/* 1005 */		PE_C_REQ_GENEOFF = 1005 ,
	/* 1006 */		PE_C_REQ_GENEON = 1006 ,
	/* 1007 */		PE_C_REQ_JOIN = 1007 ,
	/* 1008 */		PE_C_REQ_LOGIN = 1008 ,
	/* 1009 */		PE_C_REQ_OUTSTAGE = 1009 ,
	/* 1010 */		PE_C_REQ_OWNLOOK = 1010 ,
	/* 1011 */		PE_C_REQ_OWNMOVE = 1011 ,
	/* 1012 */		PE_C_REQ_SEARCHROOM = 1012 ,
	/* 1013 */		PE_C_REQ_SELECTPART = 1013 ,
	/* 1014 */		PE_C_REQ_SITDOWN = 1014 ,
	/* 1015 */		PE_C_REQ_STANDUP = 1015 ,
	/* 1016 */		PE_C_REQ_TRAPON = 1016 ,
	/* 1017 */		PE_C_REQ_WALKOFF = 1017 ,
	/* 1018 */		PE_C_REQ_WALKON = 1018 ,
	/* 1019 */		PE_S_ANS_ADDUSERVIEW = 1019 ,
	/* 1020 */		PE_S_ANS_ALIVEUSERSTATEUPDATE = 1020 ,
	/* 1021 */		PE_S_ANS_DEFEATSTATE = 1021 ,
	/* 1022 */		PE_S_ANS_ENTERROOMFAIL = 1022 ,
	/* 1023 */		PE_S_ANS_ENTERROOMSUCC = 1023 ,
	/* 1024 */		PE_S_ANS_GENONSUCC = 1024 ,
	/* 1025 */		PE_S_ANS_GENSTATEUPDATE = 1025 ,
	/* 1026 */		PE_S_ANS_HITJUDGE = 1026 ,
	/* 1027 */		PE_S_ANS_JOINFAIL = 1027 ,
	/* 1028 */		PE_S_ANS_JOINSUCC = 1028 ,
	/* 1029 */		PE_S_ANS_LOGINFAIL = 1029 ,
	/* 1030 */		PE_S_ANS_LOGINSUCC = 1030 ,
	/* 1031 */		PE_S_ANS_OUTROOMSUCC = 1031 ,
	/* 1032 */		PE_S_ANS_SUBUSERVIEW = 1032 ,
	/* 1033 */		PE_S_ANS_TRAPBUILDFAIL = 1033 ,
	/* 1034 */		PE_S_ANS_TRAPBUILDSUCC = 1034 ,
	/* 1035 */		PE_S_ANS_TRAPEXECUTE = 1035 ,
	/* 1036 */		PE_S_ANS_USERMOVE = 1036 ,
	/* 1037 */		PE_S_ANS_WINSTATE = 1037 ,
	/* 1038 */		PE_S_BRD_GAMESTARTSUCC = 1038 ,
	/* 1039 */		PE_S_BRD_OUTGAMESTATE = 1039 ,
	/* 1040 */		PE_S_BRD_ROOMSTATE = 1040 ,
	/* 1041 */		PE_S_NTF_OVERLAPCRUSH = 1041 ,
	/* 1042 */		PE_S_ANS_SELECTPARTSUCC = 1042,
	/* 1043 */		PE_S_NTF_ROOMSEARCHNOTROLE = 1043,
};
