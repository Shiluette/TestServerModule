#include"stdafx.h"
#include"SessionManager.h"

SessionManager::SessionManager() : _lock(L"SessionManager")
{
	_idSeed = 1;
	_MaxConnection = MAXCONNECTIONNUMBER;
	
}

SessionManager::SessionManager(int maxConnection) : _lock(L"SessionManager")
{
	_idSeed = 1;
	_MaxConnection = maxConnection;
}

SessionManager::~SessionManager()
{
	vector<Session *>removeSessionVec;
	removeSessionVec.resize(_sessionList.size());
	std::copy(_sessionList.begin(), _sessionList.end(), removeSessionVec.begin());
	for (auto session : removeSessionVec) {
		session->onClose();
	}
	_sessionList.clear();
}

oid_t SessionManager::createSessionId()
{
	return _idSeed++;
}

bool SessionManager::addSession(Session * session)
{
	SAFE_LOCK(_lock);
	auto findSession = find(_sessionList.begin(), _sessionList.end(), session);
	if (findSession != _sessionList.end()) return false;
	if (_sessionCount > _MaxConnection) {
		// TODO : 적정 사용자 인원 초과 추후 대응 필요.
		// (room Server의 경우)다른 서버로 연결을 우회하게끔 패킷을 보낸다던가...
		return false;
	}
	session->setId(this->createSessionId());
	_sessionList.emplace_back(session);
	++_sessionCount;
	return true;
}

list<Session*>& SessionManager::sessionList()
{
	return _sessionList;
}

bool SessionManager::closeSession(Session * session)
{
	// TODO : Session을 닫을 때 수상한 Session과 정상인 Session에 대한 처리를 다르게 해야한다.
	// 
	if (session == nullptr) {
		// TODO : Log에 빈 Session을 보낸 상황임으로 
		// 로직상 문제 있을 가능성 유추 가능
		return false;
	}
	SAFE_LOCK(_lock);
	auto findSession = find(_sessionList.begin(), _sessionList.end(), session);
	if (findSession != _sessionList.end()) {
		// 1.소켓을 먼저 끄고
		// 2. 카운트를 바꿔준뒤,
		// 3. 객체를 삭제.
		// TODO : 객체를 지우기전에 유저 정보 최신 업데이트가 필요.
		Session * deleteSession = *findSession;
		::closesocket(deleteSession->getSocket());
		_sessionList.remove(deleteSession);
		--_sessionCount;
		SAFE_DELETE(deleteSession);
		return true;
	}
	return false;
}

Session * SessionManager::session(oid_t id)
{
	SAFE_LOCK(_lock);
	Session * findSession = nullptr;
	for (auto session : _sessionList) {
		if (session->getId() == id) {
			findSession = session;
			break;
		}

	}
	return findSession;
}
