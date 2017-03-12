#pragma once
#include"stdafx.h"
#include"Packet.h"
#include"PacketAnalyzer.h"
#include"PacketFactory.h"

Packet * PacketAnalyzer::analyzer(const char * rPacket, size_t size)
{
	size_t offset = 0;
	PacketType type = (PacketType)0;
	memcpy_s((void*)&type, sizeof(type), (void*)rPacket, sizeof(type));
	offset += sizeof(type);

	Packet * packet = PacketFactory::getInstance().getPacket(type);

	if (packet) {
		if (offset < size) {
			Stream stream((UCHAR *)(rPacket + offset), size - offset);
			packet->decode(stream);
		}
	}

	return packet;
}
