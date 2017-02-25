#pragma once
#include"stdafx.h"

class PacketAnalyzer : public Singleton<PacketAnalyzer>
{
public:
	Packet * analyzer(const char *rPacket, size_t size);
};