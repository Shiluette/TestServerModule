#include"stdafx.h"
#include"User.h"
#include"UserManager.h"

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

INT64 UserManager::login(UINT64 oid)
{
	if (!_logoutindex.empty()) {
		_lock.lock(_FILENAME, __LINE__);
		int index = *_logoutindex.begin();
		_logoutindex.erase(_logoutindex.begin());
		auto result = _login.insert(index);
		_lock.Unlock();
		if (result.second == false) SLog(L" !!!! UserManager Login Error !!!");
		_userPool[index]->set(oid, index);
		return index;
	}
	for (auto i = 0; i < 500; ++i) {
		if (_userPool[i]->oid() != -1) continue;
		_userPool[i]->set(oid, i);
		_lock.lock(_FILENAME, __LINE__);
		auto result = _login.insert(i);
		_lock.Unlock();
		return i;
	}
	return -1;
}

void UserManager::logout(INT64 uid)
{
	SAFE_LOCK(_lock);
	_login.erase(uid);
	_logoutindex.insert(uid);
	_userPool[uid]->clear();
}

User * UserManager::getUser(INT64 uid)
{
	return _userPool[uid];
}
