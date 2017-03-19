#include"stdafx.h"
#include "RoomProcess.h"

void IngameProcess::TPacket_STARTUSERDATA(Session * session, Packet * rowPacket)
{
	PK_T_NTF_STARTUSERDATA * packet = (PK_T_NTF_STARTUSERDATA *)rowPacket;
	 = packet->roomNumber
}
