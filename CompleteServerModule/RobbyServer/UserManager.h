#pragma once
#include"stdafx.h"

class User;
class UserManager : public  Singleton<UserManager>
{
private:
	std::array<User *, 2000> _userPool;
	std::set<INT64> _login;
	std::set<INT64> _logoutindex;
	Lock _lock;
public:
	UserManager();
	virtual ~UserManager();
	INT64 login(UINT64 & oid, wstr_t & id);
	void logout(INT64 uid);
	User * getUser(INT64  uid);
};