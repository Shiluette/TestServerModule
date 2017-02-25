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
	void setRole(USER_ROLE role);
	int role();	// [0] : None [1] : Killer  [2] : surviors
	INT64 & roomNumber();
	UINT64 & oid();
	INT64 & uid();
	void clear();
};
class UserManager : Singleton<UserManager>
{
private:
	std::array<User *, 2000> _userPool;
	std::set<int> _login;
	std::set<int> _logoutindex;
	Lock _lock;
public:
	UserManager();
	virtual ~UserManager();
	void login(UINT64 oid);
	void logout(INT64 uid);
};
typedef enum
{
	ROOM_FULL,
	SURVIORS_LEAK,
	KILLER_LEAK,
	BOTH_LEAK,
}ROOM_STATE;
class Room
{
	ROOM_STATE _state;
	std::vector<User *> _enteredUser;
	INT64 _roomNumber;
public:
	Room();
	~Room();
	bool enter(User * user);
	bool leave(User * user);
	int state();
	int setState(ROOM_STATE);
	INT64 roomNumber();
	bool isEnter(int role);
public:
	volatile int _allcnt;
	volatile int _kilCnt;
	volatile int _surCnt;
};
class RoomManager : public Singleton<RoomManager>
{
	typedef std::pair<User *, bool> EntOutUser;	// second [true] : 입장 : false : 나가기
	typedef std::pair<volatile int *, int> index_;
	std::array<Room *, 500>_totalRoom;
	std::vector<index_> _UserCntFerIndex;
	ThreadJobQueue<EntOutUser *>  * _enterUserQueue;
	Lock _lock;
public:
	RoomManager();
	virtual ~RoomManager();
	void addEnterUser(User * user);
	void LeaveUser(User * user);
	void execute();
};

class EnterRoomWork : public Work
{
	void tick()
	{
		RoomManager::getInstance().execute();
	}
};