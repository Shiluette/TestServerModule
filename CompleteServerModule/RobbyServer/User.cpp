#include "stdafx.h"
#include "User.h"

User::User()
{
	_oid = 0;
	_uid = -1;
	_state = STATE_NONE;
	_role = ROLE_NONE;
	_roomNumber = 7000;
}

User::~User()
{
	SLog(L"User Pointer Delete oid : [%I64u] , uid : [%I64d]", _oid, _uid);
}

void User::set(UINT64 oid, INT64 uid, wstr_t id, USER_STATE state, USER_ROLE role, INT64 roomnumber)
{
	_oid = oid;
	_uid = uid;
	_id.assign(id.begin(), id.end());
	_state = state;
	_role = role;
	_roomNumber = roomnumber;
}

void User::enterRoom(int roomNumber)
{
	if (_state == STATE_ROOM || _state == STATE_GAMEING) {
		SLog(L"!! Enter room error oid : [%I64u] , uid : [%I64d]", _oid, _uid);
		return;
	}
	_roomNumber = roomNumber;
	_state = STATE_ROOM;
	SLog(L"!! Enter room oid : [%I64u] , uid : [%I64d], Room Number : [ %d ]", _oid, _uid, roomNumber);
}

void User::leaveRoom()
{
	SLog(L"!! Leave room oid : [%I64u] , uid : [%I64d], Room Number : [ %I64d ]", _oid, _uid, _roomNumber);
	_roomNumber = 7000;
	_state = STATE_ROBBY;
}

USER_ROLE User::setRole(USER_ROLE role)
{
	_role = role;
	return _role;
}

USER_ROLE & User::role()
{
	return _role;
	/*switch (_role)
	{
	case ROLE_NONE:
		return 0;
	case ROLE_SURVIORS : 
		return 2;
	case ROLE_KILLER : 
		return 1;
	}
	return 999;*/
}

INT64 & User::roomNumber()
{
	return _roomNumber;
}

UINT64 & User::oid()
{
	UINT64 id = VolatileCast(_oid);
	return id;
}

INT64 & User::uid()
{
	return _uid;
}

wstr_t & User::id()
{
	return _id;
}

void User::clear()
{
	_oid = 0;
	_uid = -1;
	_state = STATE_NONE;
	_role = ROLE_NONE;
	_roomNumber = 7000;
}
