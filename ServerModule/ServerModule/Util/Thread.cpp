#include"stdafx.h"
#include"Thread.h"

Thread::Thread(thread_t * thread, wstr_t name)
{
	std::hash<std::thread::id> hasher;
	_name = name;
	_thread = thread;
	_id = hasher(_thread->get_id());

	ThreadManager::getInstance().put(this); // ThreadManager등록 1
}

Thread::~Thread()
{
	_thread->join();
	SAFE_DELETE(_thread);
	SAFE_DELETE(_lock);
}

size_t Thread::GetId()
{
	return _id;
}

wstr_t & Thread::GetName()
{
	return _name;
}

void Thread::SetLock(Lock * lock)
{
	_lock = lock;
}

Lock * Thread::Getlock()
{
	return _lock;
}

ThreadManager::~ThreadManager()
{
	for (auto thread : _threadPool) {
		SAFE_DELETE(thread.second);
	}
}

void ThreadManager::put(Thread * thread)
{
	_threadPool.emplace(thread->GetId(), thread);  // ThreadManager등록 2
	SLog(L"** create thread: id[0x%x] || name[%s], pool size[%d]", 
		thread->GetId(), thread->GetName().c_str(), _threadPool.size());
}

void ThreadManager::remove(size_t id)
{
	auto iter = _threadPool.find(id);
	if (iter == _threadPool.end()) {
		return;
	}
	auto thread = iter->second;
	_threadPool.erase(iter);  // ThreadManager 삭제 1
}

Thread * ThreadManager::at(size_t id) // Thread return;
{
	if (_threadPool.empty()) {
		return nullptr;
	}
	auto iter = _threadPool.find(id);
	if (iter == _threadPool.end()) {
		return nullptr;
	}
	auto thread = iter->second;
	return thread;
}
