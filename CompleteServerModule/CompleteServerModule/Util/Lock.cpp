#include "stdafx.h"
#include "Lock.h"



Lock::Lock(WCHAR * name)
{
	_lockId = LockManager::getInstance().GeneralId();
	_name = name;

	this->_cheakingFile.clear();
	this->_cheakingLine = NULL;
}

Lock::~Lock()
{
	_name.clear();
}

const WCHAR * Lock::Name()
{
	return _name.c_str();
}

size_t Lock::LockId()
{
	return _lockId;
}

lock_t & Lock::Mutex()
{
	return _mutex;
}

void Lock::lock(LPCWSTR fileName, int lineNumber)
{
	_mutex.lock();

	_cheakingFile = fileName;
	_cheakingFile = lineNumber;

}

void Lock::Unlock()
{
	_mutex.unlock();

	_cheakingFile.clear();
	_cheakingLine = NULL;
}

void Lock::SetThreadId(size_t id)
{
	this->_threadId = id;
}

size_t Lock::ThreadId()
{
	return _threadId;
}

LockManager::LockManager()
{
	_idSeed = 0;
}

Lock * LockManager::SearchLockCycle(Lock * newLock)
{
	// thread : 현재 사용하고있는 스레드
	Thread *thread = ThreadManager::getInstance().at(_HashId(std::this_thread::get_id()));
	if (!thread) {
		return nullptr;
	}
	vector<Lock *> trace;
	trace.emplace_back(newLock);
	// newLock : 새로 락을 거는 mutex

	Lock *deadLock = nullptr;
	while (true) {
		Lock * threadLock = thread->Getlock();	// 돌고 있는 스레드의 lock (생성주기는 밖에서..)
		if (threadLock == nullptr) { break; } // 현재 스레드 -> 락이 안 걸려있는 상태
		if (threadLock->LockId() == trace[0]->LockId()) {
			// 현 스레드의 뮤텍스와 새로 걸려하는 뮤텍스가 같은 상황!!
			deadLock = threadLock;
			break;
		}
		trace.emplace_back(threadLock);
		thread = ThreadManager::getInstance().at(threadLock->ThreadId());
		// 같은 Lock을 사용하고 있는 스레드를 스레드 매니저에서 찾는다.
		if (!thread) {
			break;
		}
	}
	trace.empty();
	return deadLock; 
	// return value : nullptr(NoDeadLock) or pointer(DeadLock!)
}

Lock * LockManager::CheckDeadLock(Lock * Thislock)
{
	Lock *deadLock = this->SearchLockCycle(Thislock);
	if (deadLock) {
		return deadLock;
	}
	return nullptr;
}

size_t LockManager::GeneralId()
{
	// 안정성 위해!
	size_t id = _idSeed++;
	// 락걸기 싫어요.
	return id;
}

LockSafe::LockSafe(Lock * lock, LPCWSTR fileName, int lineNo)
{
	if (lock == nullptr) {
		return;
	}
	_lock = lock;
	Lock * deadLock = LockManager::getInstance().SearchLockCycle(_lock);
	if (deadLock != nullptr) {
		// TODO : 데드락 상황. 에러 표시 필요.
	}
	_lock->lock(fileName, lineNo);
	_lock->SetThreadId(LockManager::getInstance()._HashId(std::this_thread::get_id()));
}

LockSafe::~LockSafe()
{
	if (!_lock) {
		return;
	}
	_lock->Unlock();
	_lock->SetThreadId(0);
}
