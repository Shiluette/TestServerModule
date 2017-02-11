#pragma once
#include "stdafx.h" 
#include"PacketHeader.h"
#include"Packet.h"




class PacketFactory : public Singleton<PacketFactory>
{
public:
	Packet* getPacket(Int64 packetType) {
		switch(packetType) {
			case PE_C_Dad:		return new PK_C_Dad();
			case PE_C_HI:		return new PK_C_HI();
			case PE_C_Metoo:		return new PK_C_Metoo();
		}
		return nullptr;
	}
};
