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
}

void User::set(UINT64 oid, INT64 uid, USER_STATE state, USER_ROLE role, INT64 roomnumber)
{
	_oid = oid;
	_uid = uid;
	_state = state;
	_role = role;
	_roomNumber = roomnumber;
}

void User::enterRoom(int roomNumber)
{
	if (_state == STATE_ROOM || _state == STATE_GAMEING) {
		SLog(L"!! Enter room error oid : [%d] , uid : [%d]", &_oid, &_uid);
		return;
	}
	_roomNumber = roomNumber;
	_state = STATE_ROOM;
	SLog(L"!! Enter room oid : [%d] , uid : [%d], Room Number : [ %d ]", &_oid, &_uid, &roomNumber);
}

void User::leaveRoom()
{
	SLog(L"!! Leave room oid : [%d] , uid : [%d], Room Number : [ %d ]", &_oid, &_uid, &_roomNumber);
	_roomNumber = 7000;
	_state = STATE_ROBBY;
}

void User::setRole(USER_ROLE role)
{
	_role = role;
}

int User::role()
{
	switch (_role)
	{
	case ROLE_NONE:
		return 0;
	case ROLE_SURVIORS : 
		return 2;
	case ROLE_KILLER : 
		return 1;
	}
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

void User::clear()
{
	_oid = 0;
	_uid = -1;
	_state = STATE_NONE;
	_role = ROLE_NONE;
	_roomNumber = 7000;
}

Room::Room()
{
	_kilCnt = 0;
	_surCnt = 0;
	static int roomcnt = 1;
	_roomNumber = roomcnt++;
	_allcnt = 0;
}

Room::~Room()
{
	for (auto iter = _enteredUser.begin(); iter != _enteredUser.end(); ++iter) {
		SAFE_DELETE(*iter);
	}
	_enteredUser.clear();
}

bool Room::enter(User * user)
{
	if (user->role() == 1) ++_kilCnt;
	else
		++_surCnt;
	_enteredUser.push_back(user);
	user->enterRoom(_roomNumber);

	try {
		if (_kilCnt < 1 && _surCnt < 3)		throw 1;		// both
		if (_kilCnt < 1 && _surCnt > 2)		throw 2;		// killer
		if (_kilCnt == 1 && _surCnt < 3)	throw 3;		// surviors
		if (_kilCnt == 1 && _surCnt == 3)	throw 4;		// full
	}
	catch (int num) {
		switch (num)
		{
		case 1:
			_state = BOTH_LEAK;
			_allcnt = _kilCnt + _surCnt;
			break;
		case 2:
			_state = KILLER_LEAK;
			_allcnt = _kilCnt + _surCnt;
			break;
		case 3:
			_state = SURVIORS_LEAK;
			_allcnt = _kilCnt + _surCnt;
			break;
		case 4:
			_state = ROOM_FULL;
			_allcnt = -1;
			break;
		default:
			return false;
		}
	}
	return true;
}

bool Room::leave(User * user)
{
	auto find = std::find(_enteredUser.begin(), _enteredUser.end(), user);
	if (find == _enteredUser.end()) return false;
	_enteredUser.erase(find);
	user->role() == 1 ? --_kilCnt : --_surCnt;
	user->leaveRoom();
	try {
		if (_kilCnt < 1 && _surCnt < 3)	throw 1;		// both
		if (_kilCnt < 1 && _surCnt > 2)	throw 2;		// killer
		if (_kilCnt == 1 && _surCnt < 3)	throw 3;		// surviors
		//if (_kilCnt == 1 && _surCnt == 3) throw 4;		// full
	}
	catch (int num) {
		switch (num)
		{
		case 1:
			_state = BOTH_LEAK;
			_allcnt = _kilCnt + _surCnt;
			break;
		case 2:
			_state = KILLER_LEAK;
			_allcnt = _kilCnt + _surCnt;
			break;
		case 3:
			_state = SURVIORS_LEAK;
			_allcnt = _kilCnt + _surCnt;
			break;
		default:
			return false;
		}
	}
	return true;
}

int Room::state()
{
	return 0;
}

int Room::setState(ROOM_STATE)
{
	return 0;
}

INT64 Room::roomNumber()
{
	return INT64();
}

bool Room::isEnter(int role)
{
	if (_state == BOTH_LEAK) return true;
	if (role == 1 && _state == KILLER_LEAK ) { // killer
		return true;
	}
	if (role == 2 && _state == SURVIORS_LEAK) { // surviors
		return true;
	}
	return false;
}

RoomManager::RoomManager() : _lock(L"RoomManager")			
{
	_UserCntFerIndex.resize(500);
	// Queue 생성
	_enterUserQueue = new ThreadJobQueue<EntOutUser *>(L"RoomManager");
	for (int i = 0; i < 500; ++i) {
		_totalRoom[i] = new Room();
		index_ in( &_totalRoom[i]->_allcnt ,i+1);
		_UserCntFerIndex.push_back(in);
	}
}

RoomManager::~RoomManager()
{
	for (auto iter : _roomPool)
		SAFE_DELETE(iter)
	_roomPool.clear();
	SAFE_DELETE(_enterUserQueue);
}

void RoomManager::addEnterUser(User * user)
{
	EntOutUser * Userpair = new EntOutUser;
	Userpair->first = user;
	Userpair->second = true;
	SAFE_LOCK(_lock);
	_enterUserQueue->push(Userpair);
}

void RoomManager::LeaveUser(User * user)
{
	EntOutUser  * Userpair = new EntOutUser;
	Userpair->first = user;
	Userpair->second = false;
	SAFE_LOCK(_lock);
	_enterUserQueue->push(Userpair);
}

void RoomManager::execute()
{
	// Queue에 들어있는 유저 인원수
	size_t size = _enterUserQueue->size();
	if (size == 0) return;			// 입출력 없으므로 종료
	// 인원수가 array크기보다 클 경우 array 인덱스 만큼 작업
	if (size > SIZE_64) size =  64;
	// _UserCntFerIndex sorting
	std::sort(_UserCntFerIndex.begin(), _UserCntFerIndex.end(),
		[](auto &left, auto &right){ return *(left.first) > *(right.first)});
	
	std:: array<EntOutUser *, SIZE_128> popEntQut;
	popEntQut.fill(nullptr);
	_lock.lock(_FILENAME, __LINE__);
	// Queue -> array로 추출
	for(int i = 0; i < size ; ++i){
		_enterUserQueue->pop(popEntQut[i]);
	}
	_lock.Unlock();
	
	User * popUser = nullptr;
	// 빼온 EntQutUser데이터 대조
	for (int i = 0; i < size; ++i) {
		popUser = popEntQut[i]->first;
		// Enter 시
		if (popEntQut[i]->second == true) {
			int uRole = popUser->role();
			for (auto mindex : _UserCntFerIndex) {
				if (!_totalRoom[mindex.second]->isEnter(uRole))continue;			// in Room.....
				_totalRoom[mindex.second]->enter(popUser);
			}
		}
		// Leave 시
		if (popEntQut[i]->second == false){
			_totalRoom[popUser->roomNumber()]->leave(popUser);
		
		}
	}
	for (auto i : popEntQut)
		SAFE_DELETE(i);
}

UserManager::UserManager() : _lock(L"UserManager")
{
	for (auto user : _userPool)
		user = new User();
	SLog(L"** _userPool 'new' Create **");
}

UserManager::~UserManager()
{
	for (auto user : _userPool)
		SAFE_DELETE(user);
	SLog(L"** _userPool ALL DELETE **");
}

void UserManager::login(UINT64 oid)
{
	if (!_logoutindex.empty()) {
		_lock.lock(_FILENAME, __LINE__);
		int index = *_logoutindex.begin();
		_logoutindex.erase(_logoutindex.begin());
		auto result = _login.insert(index);
		_lock.Unlock();
		if (result.second == false) SLog(L" !!!! UserManager Login Error !!!");
		_userPool[index]->set(oid, index);
		return;
	}
	for (auto i = 0; i < 500; ++i) {
		if (_userPool[i]->oid() != -1) continue;
		_userPool[i]->set(oid, i);
		_lock.lock(_FILENAME, __LINE__);
		auto result = _login.insert(i);
		_lock.Unlock();
		return;
	}
}

void UserManager::logout(INT64 uid)
{
	SAFE_LOCK(_lock);
	_login.erase(uid);
	_logoutindex.insert(uid);
	_userPool[uid]->clear();
}
