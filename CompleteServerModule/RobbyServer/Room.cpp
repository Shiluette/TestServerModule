#include"stdafx.h"
#include"ServerHeader.h"
#include"User.h"
#include"Room.h"


Room::Room()
{
	_state = BOTH_LEAK;
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

std::vector<User*> * Room::UserInfo()
{
	std::vector<User *>  * info = &_enteredUser;

	return info;
}



						/*    RoomManager      */



RoomManager::RoomManager() : _lock(L"RoomManager")
{
	_UserCntFerIndex.reserve(500);//resize(500);
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
				if (!_totalRoom[mindex.second - 1]->isEnter(uRole))continue;			// in Room.....
				_totalRoom[mindex.second - 1]->enter(popUser);
				SLog(L"~~~ Enter Room To User .[UID : %d ] [ROOMNUMBER : %d ] [ROLE : %d] ~~~", popUser->uid(), popUser->roomNumber(), popUser->role());
				std::vector<User *> * UserInfo = _totalRoom[mindex.second - 1]->UserInfo();
				PK_S_ANS_ENTERROOMSUCC cPacket;
				cPacket.roomNumber = mindex.second;
				cPacket.ucount = (BYTE)UserInfo->size();
				for (auto iter = 0; iter < cPacket.ucount; ++iter) {
					cPacket.uid[iter] = (*UserInfo)[iter]->uid();
					cPacket.id[iter].assign((*UserInfo)[iter]->id());
					cPacket.role[iter] = (BYTE)(*UserInfo)[iter]->role();
				}
				// 진입하는 유저에게 방에 대한 데이터 송신
				Session * session = SessionManager::getInstance().session(popUser->oid());
				session->sendPacket(&cPacket);
				PK_S_BRD_ROOMSTATE sPacket;
				sPacket.uid = popUser->uid();
				sPacket.update = (uRole == ROLE_KILLER) ? 10 : 11;
				sPacket.id.assign(popUser->id());
				// 기존 방에 있던 유저들에게 신규 유저 데이터 송신
				for (auto i : *UserInfo) {
					if (popUser->oid() == i->oid()) continue;
					session = SessionManager::getInstance().session(i->oid());
					session->sendPacket(&sPacket);
				}
				break;
			}
		}
		// Leave 시
		if (popEntQut[i]->second == false) {
			INT64 roomNumber = popUser->roomNumber();
			if (_totalRoom[roomNumber]->leave(popUser)) {
				// 나가기를 시도한 유저에게 나가기 성공 패킷을 송신
				PK_S_ANS_OUTROOMSUCC cPacket;
				PK_S_BRD_ROOMSTATE sPacket;
				sPacket.uid = popUser->uid();
				sPacket.update = 2;
				cPacket.uid = sPacket.uid;
				Session * session = SessionManager::getInstance().session(popUser->oid());
				session->sendPacket(&cPacket);
				// 방의 변경 정보를 방에 있는 유저들에세 송신
				std::vector<User *> * UserInfo = _totalRoom[roomNumber]->UserInfo();
				for (auto i : *UserInfo) {
					session = SessionManager::getInstance().session(i->oid());
					session->sendPacket(&sPacket);
				}

			}
		}
	}
	for (auto i = 0; i < popEntQut.size(); ++i) {
		SAFE_DELETE(popEntQut[i]);
	}
}

Room * RoomManager::findRoom(int roomNumber)
{
	return _totalRoom[roomNumber - 1];
}

AutoIoSupervise::AutoIoSupervise()
{
	const int AUTO_IO_SUPERVISE = 1;
	TaskNode * node = new TaskNode(this, AUTO_IO_SUPERVISE, TICK_INFINTY);
	TaskManager::getInstance().add(node);
}
