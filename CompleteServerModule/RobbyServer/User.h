#pragma once
#include"stdafx.h"
typedef enum
{
	STATE_NONE,
	STATE_ROBBY,
	STATE_ROOM,
	STATE_GAMEING,
}USER_STATE;
typedef enum
{
	ROLE_NONE,
	ROLE_KILLER,
	ROLE_SURVIORS,
}USER_ROLE;
class User
{
private:
	volatile UINT64 _oid; // Session에서 받은 id값
	INT64 _uid; // Usermanager에서 받은 id값 
	USER_STATE _state;
	USER_ROLE _role;
	INT64 _roomNumber;
public:
	User();
	~User();
	void set(UINT64 oid, INT64 uid, USER_STATE state = STATE_ROBBY, 
		USER_ROLE role = ROLE_NONE, INT64 roomnumber = 7000);
	void enterRoom(int roomNumber);
	void leaveRoom();
	USER_ROLE setRole(USER_ROLE role);
	int role();	// [0] : None [1] : Killer  [2] : surviors
	INT64 & roomNumber();
	UINT64 & oid();
	INT64 & uid();
	void clear();
};


