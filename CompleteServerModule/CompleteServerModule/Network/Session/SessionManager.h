#pragma once
#include"stdafx.h"
#include"Session.h"

#define MAXCONNECTIONNUMBER 1000

class SessionManager : public Singleton<SessionManager>
{
	typedef list<Session *> SessionList;

	SessionList _sessionList;
	int _sessionCount;
	int _MaxConnection;
	Lock _lock;
	oid_t _idSeed;

	typedef std::function<void(SessionList * sessionList, wstr_t *arg)> cmdFunc;
	unordered_map<wstr_t, cmdFunc> _serverCommend;

public:
	SessionManager();
	SessionManager(int maxConnection);
	~SessionManager();

	oid_t createSessionId();
	bool addSession(Session * session);
	
	list<Session*> & sessionList();

	bool closeSession(Session * session);
	Session *session(oid_t id);
};