#pragma once
#include"stdafx.h"
#include "PacketHeader.h"
#include"Stream.h"


class Packet {
 public:
	virtual PacketType type() = 0;
	virtual void encode(Stream &stream) {stream << (Int64)this->type();}
	virtual void decode(Stream &stream){ };
};


class PK_C_REQ_ATT : public Packet
{
public:
	PacketType type() {return PE_C_REQ_ATT;}
	Int64 uid;
	//std::vector<FLOAT> pos;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
		//stream << pos;
	}

	void decode(Stream &stream) {
		stream >> &uid;
		//stream >> &pos;
	}

};

class PK_C_REQ_EXITROOM : public Packet
{
public:
	PacketType type() {return PE_C_REQ_EXITROOM;}
	Int64 uid;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
	}

	void decode(Stream &stream) {
		stream >> &uid;
	}

};

class PK_C_REQ_GAMEOUT : public Packet
{
public:
	Int64 uid;
	PacketType type() {return PE_C_REQ_GAMEOUT;}
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
	}
	void decode(Stream &stream) {
		stream >> &uid;
	}
};

class PK_C_REQ_GAMEREADYOFF : public Packet
{
public:
	PacketType type() {return PE_C_REQ_GAMEREADYOFF;}
	Int64 uid;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
	}

	void decode(Stream &stream) {
		stream >> &uid;
	}

};

class PK_C_REQ_GAMEREADYON : public Packet
{
public:
	PacketType type() {return PE_C_REQ_GAMEREADYON;}
	Int64 uid;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
	}

	void decode(Stream &stream) {
		stream >> &uid;
	}

};

class PK_C_REQ_GENEOFF : public Packet
{
public:
	PacketType type() {return PE_C_REQ_GENEOFF;}
	Int64 uid;
	//std::vector<FLOAT> pos;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
		//stream << pos;
	}

	void decode(Stream &stream) {
		stream >> &uid;
		//stream >> &pos;
	}

};

class PK_C_REQ_GENEON : public Packet
{
public:
	PacketType type() {return PE_C_REQ_GENEON;}
	Int64 uid;
	//std::vector<FLOAT> pos;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
		//stream << pos;
	}

	void decode(Stream &stream) {
		stream >> &uid;
		//stream >> &pos;
	}

};

class PK_C_REQ_JOIN : public Packet
{
public:
	PacketType type() {return PE_C_REQ_JOIN;}
	wstr_t id;
	wstr_t pw;
	wstr_t name;
	wstr_t email;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << id;
		stream << pw;
		stream << name;
		stream << email;
	}

	void decode(Stream &stream) {
		stream >> &id;
		stream >> &pw;
		stream >> &name;
		stream >> &email;
	}

};

class PK_C_REQ_LOGIN : public Packet
{
public:
	PacketType type() {return PE_C_REQ_LOGIN;}
	wstr_t id;
	wstr_t pw;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << id;
		stream << pw;
	}

	void decode(Stream &stream) {
		stream >> &id;
		stream >> &pw;
	}

};

class PK_C_REQ_OUTSTAGE : public Packet
{
public:
	PacketType type() {return PE_C_REQ_OUTSTAGE;}
	Int64 uid;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
	}

	void decode(Stream &stream) {
		stream >> &uid;
	}

};

class PK_C_REQ_OWNLOOK : public Packet
{
public:
	PacketType type() {return PE_C_REQ_OWNLOOK;}
	Int64 uid;
	/*std::vector<FLOAT> rotatep;*/
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
		/*stream << rotatep;*/
	}

	void decode(Stream &stream) {
		stream >> &uid;
		/*stream >> &rotatep;*/
	}

};

class PK_C_REQ_OWNMOVE : public Packet
{
public:
	PacketType type() {return PE_C_REQ_OWNMOVE;}
	Int64 uid;
	BYTE posallow;
	/*std::vector<FLOAT> pos;*/
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
		stream << posallow;
		//stream << pos;
	}

	void decode(Stream &stream) {
		stream >> &uid;
		stream >> &posallow;
		//stream >> &pos;
	}

};

class PK_C_REQ_SEARCHROOM : public Packet
{
public:
	PacketType type() {return PE_C_REQ_SEARCHROOM;}
	Int64 uid;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
	}

	void decode(Stream &stream) {
		stream >> &uid;
	}

};

class PK_C_REQ_SELECTPART : public Packet
{
public:
	PacketType type() {return PE_C_REQ_SELECTPART;}
	Int64 uid;
	BYTE  role;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
		stream << role;
	}

	void decode(Stream &stream) {
		stream >> &uid;
		stream >> &role;
	}

};

class PK_C_REQ_SITDOWN : public Packet
{
public:
	PacketType type() {return PE_C_REQ_SITDOWN;}
	Int64 uid;
	//std::vector<FLOAT> pos;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
		//stream << pos;
	}

	void decode(Stream &stream) {
		stream >> &uid;
		//stream >> &pos;
	}

};

class PK_C_REQ_STANDUP : public Packet
{
public:
	PacketType type() {return PE_C_REQ_STANDUP;}
	Int64 uid;
	//std::vector<FLOAT> pos;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
	//	stream << pos;
	}

	void decode(Stream &stream) {
		stream >> &uid;
	//	stream >> &pos;
	}

};

class PK_C_REQ_TRAPON : public Packet
{
public:
	PacketType type() {return PE_C_REQ_TRAPON;}
	Int64 uid;
	//std::vector<FLOAT> pos;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
		//stream << pos;
	}

	void decode(Stream &stream) {
		stream >> &uid;
		//stream >> &pos;
	}

};

class PK_C_REQ_WALKOFF : public Packet
{
public:
	PacketType type() {return PE_C_REQ_WALKOFF;}
	Int64 uid;
	//std::vector<FLOAT> pos;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
		//stream << pos;
	}

	void decode(Stream &stream) {
		stream >> &uid;
		//stream >> &pos;
	}

};

class PK_C_REQ_WALKON : public Packet
{
public:
	PacketType type() {return PE_C_REQ_WALKON;}
	Int64 uid;
	//std::vector<FLOAT> pos;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
	//	stream << pos;
	}

	void decode(Stream &stream) {
		stream >> &uid;
	//	stream >> &pos;
	}

};

class PK_S_ANS_ADDUSERVIEW : public Packet
{
public:
	PacketType type() {return PE_S_ANS_ADDUSERVIEW;}
	BYTE ucount;
	//std::vector<Int64> uid;
	//std::vector<FLOAT> pos;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << ucount;
	//	stream << uid;
	//	stream << pos;
	}

	void decode(Stream &stream) {
		stream >> &ucount;
	//	stream >> &uid;
	//	stream >> &pos;
	}

};
class PK_S_ANS_SELECTPARTSUCC : public Packet
{
public:
	PacketType type() { return PE_S_ANS_SELECTPARTSUCC; }
	BYTE role;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << role;
	}

	void decode(Stream &stream) {
		stream >> &role;
	}
};

class PK_S_ANS_ALIVEUSERSTATEUPDATE : public Packet
{
public:
	PacketType type() {return PE_S_ANS_ALIVEUSERSTATEUPDATE;}
	Int64 uid;
	BYTE state;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
		stream << state;
	}

	void decode(Stream &stream) {
		stream >> &uid;
		stream >> &state;
	}

};

class PK_S_ANS_DEFEATSTATE : public Packet
{
public:
	PacketType type() {return PE_S_ANS_DEFEATSTATE;}
	Int64 uid;
	BYTE role;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
		stream << role;
	}

	void decode(Stream &stream) {
		stream >> &uid;
		stream >> &role;
	}

};

class PK_S_ANS_ENTERROOMFAIL : public Packet
{
public:
	PacketType type() {return PE_S_ANS_ENTERROOMFAIL;}
};

class PK_S_ANS_ENTERROOMSUCC : public Packet
{
public:
	PacketType type() {return PE_S_ANS_ENTERROOMSUCC;}
	BYTE ucount;
	Int64 uid[4];
	wstr_t id[4];
	BYTE role[4];
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << ucount;
		for (int cnt = 0; cnt < ucount; ++cnt) {
			stream << uid[cnt];
			stream << id[cnt];
			stream << role[cnt];
		}
	}

	void decode(Stream &stream) {
		stream >> &ucount;
		for (int cnt = 0; cnt < ucount; ++cnt) {
			stream >> &(uid[cnt]);
			stream >> &(id[cnt]);
			stream >> &(role[cnt]);
		}
	}

};

class PK_S_ANS_GENONSUCC : public Packet
{
public:
	PacketType type() {return PE_S_ANS_GENONSUCC;}
	BYTE genstate;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << genstate;
	}

	void decode(Stream &stream) {
		stream >> &genstate;
	}

};

class PK_S_ANS_GENSTATEUPDATE : public Packet
{
public:
	PacketType type() {return PE_S_ANS_GENSTATEUPDATE;}
	BYTE genstate;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << genstate;
	}

	void decode(Stream &stream) {
		stream >> &genstate;
	}

};

class PK_S_ANS_HITJUDGE : public Packet
{
public:
	PacketType type() {return PE_S_ANS_HITJUDGE;}
	Int64 uid;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
	}

	void decode(Stream &stream) {
		stream >> &uid;
	}

};

class PK_S_ANS_JOINFAIL : public Packet
{
public:
	PacketType type() {return PE_S_ANS_JOINFAIL;}
	BYTE errcode;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << errcode;
	}

	void decode(Stream &stream) {
		stream >> &errcode;
	}

};

class PK_S_ANS_JOINSUCC : public Packet
{
public:
	PacketType type() {return PE_S_ANS_JOINSUCC;}
};

class PK_S_ANS_LOGINFAIL : public Packet
{
public:
	PacketType type() {return PE_S_ANS_LOGINFAIL;}
	BYTE errcode;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << errcode;
	}

	void decode(Stream &stream) {
		stream >> &errcode;
	}

};

class PK_S_ANS_LOGINSUCC : public Packet
{
public:
	PacketType type() {return PE_S_ANS_LOGINSUCC;}
	Int64 uid;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
	}

	void decode(Stream &stream) {
		stream >> &uid;
	}

};

class PK_S_ANS_OUTROOMSUCC : public Packet
{
public:
	PacketType type() {return PE_S_ANS_OUTROOMSUCC;}
	Int64 uid;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
	}

	void decode(Stream &stream) {
		stream >> &uid;
	}

};

class PK_S_ANS_SUBUSERVIEW : public Packet
{
public:
	PacketType type() {return PE_S_ANS_SUBUSERVIEW;}
	BYTE ucount;
	//std::vector<Int64> uid;
	//std::vector<FLOAT> pos;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << ucount;
	//	stream << uid;
	//	stream << pos;
	}

	void decode(Stream &stream) {
		stream >> &ucount;
	//	stream >> &uid;
	//	stream >> &pos;
	}

};

class PK_S_ANS_TRAPBUILDFAIL : public Packet
{
public:
	PacketType type() {return PE_S_ANS_TRAPBUILDFAIL;}
	Int64 uid;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
	}

	void decode(Stream &stream) {
		stream >> &uid;
	}

};

class PK_S_ANS_TRAPBUILDSUCC : public Packet
{
public:
	PacketType type() {return PE_S_ANS_TRAPBUILDSUCC;}
	Int64 uid;
	//std::vector<FLOAT> trappos;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
	//	stream << trappos;
	}

	void decode(Stream &stream) {
		stream >> &uid;
	//	stream >> &trappos;
	}

};

class PK_S_ANS_TRAPEXECUTE : public Packet
{
public:
	PacketType type() {return PE_S_ANS_TRAPEXECUTE;}
	Int64 uid;
	BYTE hp;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
		stream << hp;
	}

	void decode(Stream &stream) {
		stream >> &uid;
		stream >> &hp;
	}

};

class PK_S_ANS_USERMOVE : public Packet
{
public:
	PacketType type() {return PE_S_ANS_USERMOVE;}
	Int64 uid;
	//std::vector<FLOAT> pos;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
	//	stream << pos;
	}

	void decode(Stream &stream) {
		stream >> &uid;
	//	stream >> &pos;
	}

};

class PK_S_ANS_WINSTATE : public Packet
{
public:
	PacketType type() {return PE_S_ANS_WINSTATE;}
	Int64 uid;
	BYTE role;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
		stream << role;
	}

	void decode(Stream &stream) {
		stream >> &uid;
		stream >> &role;
	}

};

class PK_S_BRD_GAMESTARTSUCC : public Packet
{
public:
	PacketType type() {return PE_S_BRD_GAMESTARTSUCC;}
	wstr_t IP;
	INT32 PORT;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << IP;
		stream << PORT;
	}

	void decode(Stream &stream) {
		stream >> &IP;
		stream >> &PORT;
	}

};

class PK_S_BRD_OUTGAMESTATE : public Packet
{
public:
	PacketType type() {return PE_S_BRD_OUTGAMESTATE;}
	Int64 uid;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
	}

	void decode(Stream &stream) {
		stream >> &uid;
	}

};

class PK_S_BRD_ROOMSTATE : public Packet
{
public:
	PacketType type() {return PE_S_BRD_ROOMSTATE;}
	Int64 uid;
	BYTE update;
	wstr_t id;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << uid;
		stream << update;
		stream << id;
	}

	void decode(Stream &stream) {
		stream >> &uid;
		stream >> &update;
		stream >> &id;
	}

};

class PK_S_NTF_OVERLAPCRUSH : public Packet
{
public:
	PacketType type() {return PE_S_NTF_OVERLAPCRUSH;}
	wstr_t id;
	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << id;
	}

	void decode(Stream &stream) {
		stream >> &id;
	}

};
class PK_S_NTF_ROOMSEARCHNOTROLE : public Packet
{
public:
	PacketType type() { return PE_S_NTF_ROOMSEARCHNOTROLE; }
};
