#pragma once
#include"stdafx.h"

class User;
class UserManager : public  Singleton<UserManager>
{
private:
	std::array<User *, 2000> _userPool;
	std::set<int> _login;
	std::set<int> _logoutindex;
	Lock _lock;
public:
	UserManager();
	virtual ~UserManager();
	INT64 login(UINT64 oid);
	void logout(INT64 uid);
	User * getUser(INT64  uid);
};