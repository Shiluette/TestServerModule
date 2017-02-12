#pragma once
#include"stdafx.h"


// # ������(�Ű����� ��ȯ��) , ## ������(�������ڸ� �ٸ���� �̾��ٶ� ���.)
#define MAKE_THREAD(className, proces) (new Thread(new thread_t(&className##::##process, this), L#className))
#define GET_CURRENT_THREAD_ID(number)															\
{																								\
	std::hash<std::thread::id> hasher;															\
	cout << "GetCurrnet_thread �� " << hasher(std::this_thread::get_id()) << endl;				\
	*number = hasher(std::this_thread::get_id());												\
}
class Lock;
typedef std::function<void(void *)> ThreadFunction;

//  Mean  : typedef void(*ThreadFuncttion)(void *)

class Thread {
	size_t _id;
	wstr_t _name;
	thread_t * _thread;			// �����ֱ⸦ �ܺη�!
	Lock	 * _lock;

public:
	Thread(thread_t * thread, wstr_t name);
	~Thread();

	size_t GetId();
	wstr_t &GetName();

	void SetLock(Lock *lock);
	Lock * Getlock();
};

class ThreadManager : public Singleton <ThreadManager>
{
	std::map <size_t, Thread *> _threadPool;
public:
	~ThreadManager();
	void put(Thread * thread);
	void remove(size_t id);
	Thread * at(size_t id);
};