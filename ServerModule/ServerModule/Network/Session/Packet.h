#pragma once
#include"stdafx.h"
#include "PacketHeader.h"
#include"..\Util\Stream.h"


class Packet {
 public:
	virtual PacketType type() = 0;
	virtual void encode(Stream &stream) {stream << (Int64)this->type();}
	virtual void decode(Stream &stream){ };
};


class PK_C_Dad : public Packet
{
public:
	PacketType type() {return PE_C_Dad;}
};

class PK_C_HI : public Packet
{
public:
	PacketType type() {return PE_C_HI;}
	int id;
	std::string name;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << id;
		stream << name;
	}

	void decode(Stream &stream) {
		stream >> &id;
		stream >> &name;
	}

};

class PK_C_Metoo : public Packet
{
public:
	PacketType type() {return PE_C_Metoo;}
	int32_t num;
	int32_t num2;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << num;
		stream << num2;
	}

	void decode(Stream &stream) {
		stream >> &num;
		stream >> &num2;
	}

};

