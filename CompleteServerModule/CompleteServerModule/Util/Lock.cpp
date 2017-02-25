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
	// thread : ���� ����ϰ��ִ� ������
	Thread *thread = ThreadManager::getInstance().at(_HashId(std::this_thread::get_id()));
	if (!thread) {
		return nullptr;
	}
	vector<Lock *> trace;
	trace.emplace_back(newLock);
	// newLock : ���� ���� �Ŵ� mutex

	Lock *deadLock = nullptr;
	while (true) {
		Lock * threadLock = thread->Getlock();	// ���� �ִ� �������� lock (�����ֱ�� �ۿ���..)
		if (threadLock == nullptr) { break; } // ���� ������ -> ���� �� �ɷ��ִ� ����
		if (threadLock->LockId() == trace[0]->LockId()) {
			// �� �������� ���ؽ��� ���� �ɷ��ϴ� ���ؽ��� ���� ��Ȳ!!
			deadLock = threadLock;
			break;
		}
		trace.emplace_back(threadLock);
		thread = ThreadManager::getInstance().at(threadLock->ThreadId());
		// ���� Lock�� ����ϰ� �ִ� �����带 ������ �Ŵ������� ã�´�.
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
	// ������ ����!
	size_t id = _idSeed++;
	// ���ɱ� �Ⱦ��.
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
		// TODO : ����� ��Ȳ. ���� ǥ�� �ʿ�.
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
