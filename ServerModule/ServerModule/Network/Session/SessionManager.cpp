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
		// TODO : ���� ����� �ο� �ʰ� ���� ���� �ʿ�.
		// (room Server�� ���)�ٸ� ������ ������ ��ȸ�ϰԲ� ��Ŷ�� �����ٴ���...
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
	// TODO : Session�� ���� �� ������ Session�� ������ Session�� ���� ó���� �ٸ��� �ؾ��Ѵ�.
	// 
	if (session == nullptr) {
		// TODO : Log�� �� Session�� ���� ��Ȳ������ 
		// ������ ���� ���� ���ɼ� ���� ����
		return false;
	}
	SAFE_LOCK(_lock);
	auto findSession = find(_sessionList.begin(), _sessionList.end(), session);
	if (findSession != _sessionList.end()) {
		// 1.������ ���� ����
		// 2. ī��Ʈ�� �ٲ��ص�,
		// 3. ��ü�� ����.
		// TODO : ��ü�� ��������� ���� ���� �ֽ� ������Ʈ�� �ʿ�.
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
