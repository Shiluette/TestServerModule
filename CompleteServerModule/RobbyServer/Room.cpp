#include"stdafx.h"
#include"ServerHeader.h"
#include"User.h"
#include"Room.h"


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
	if (role == 1 && _state == KILLER_LEAK) { // killer
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
		index_ in(&_totalRoom[i]->_allcnt, i + 1);
		_UserCntFerIndex.push_back(in);
	}
}

RoomManager::~RoomManager()
{
	for (auto iter : _totalRoom)
		SAFE_DELETE(iter)
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
	if (size > SIZE_64) size = 64;
	// _UserCntFerIndex sorting
	std::sort(_UserCntFerIndex.begin(), _UserCntFerIndex.end(),
		[](auto &left, auto &right) { return *(left.first) > *(right.first); });

	std::array<EntOutUser *, SIZE_128> popEntQut;
	popEntQut.fill(nullptr);
	_lock.lock(_FILENAME, __LINE__);
	// Queue -> array로 추출
	for (int i = 0; i < size; ++i) {
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
		if (popEntQut[i]->second == false) {
			_totalRoom[popUser->roomNumber()]->leave(popUser);

		}
	}
	for (auto i : popEntQut)
		SAFE_DELETE(i);
}

