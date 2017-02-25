#pragma once
#include"stdafx.h"
// Lock class

// Lock사용의 추적을 위한 클래스
// 추적을 위해 !!
class Lock
{
	lock_t _mutex;
	wstr_t _name;
	size_t _lockId;
	size_t _threadId;

	wstr_t _cheakingFile;
	int _cheakingLine;
public:
	Lock(WCHAR * name);
	virtual ~Lock();

	const WCHAR* Name();
	size_t LockId();

	lock_t & Mutex();
	void lock(LPCWSTR fileName, int lineNumber);
	void Unlock();

	void SetThreadId(size_t id);
	size_t ThreadId();
};


class LockSafe {
private:
	Lock * _lock;
public:
	LockSafe(Lock *lock, LPCWSTR fileName, int lineNo);
	~LockSafe();
};

#define SAFE_LOCK(lock)		LockSafe __locksafe(&lock, _W(__FILE__), __LINE__);
class LockManager : public Singleton < LockManager >
{
	size_t _idSeed;
public:
	std::hash<thread::id> _HashId;

public:
	LockManager();

	Lock * SearchLockCycle(Lock * newLock);
	Lock * CheckDeadLock(Lock *Thislock);

	size_t GeneralId();
};