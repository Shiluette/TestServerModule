#pragma once
#include"stdafx.h"

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
	std::vector<User *> & UserInfo();
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
	Room * findRoom(int roomNumber);
};

class AutoIoSupervise : public Work
{
public:
	AutoIoSupervise();
	void tick()
	{
		RoomManager::getInstance().execute();
	}
};
static AutoIoSupervise autoiosupervisor;