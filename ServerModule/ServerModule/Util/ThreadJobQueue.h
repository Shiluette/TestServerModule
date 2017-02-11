#pragma once
#include"stdafx.h"
#include<queue>
template<class T>
class ThreadJobQueue {
private:
	enum{
		WRITE_QUEUE,
		READ_QUEUE,
		MAX_QUEUE
	};
	std::queue<T> _queue[MAX_QUEUE]; // 0 : WRITE_QUEUE, 1 : READ_QUEUE
	
	queue<T> * _writeQueue;
	queue<T> * _readQueue;

	Lock _lock;
public:
	ThreadJobQueue(WCHAR * name) : _lock(name)
	{
		_writeQueue = _queue[WRITE_QUEUE];
		_readQueue = _queue[READ_QUEUE];
	}
	~ThreadJobQueue()
	{
		if (!this->_readQueue->empty()) {
			// TODO : 예상못한 큐의 제거시 필요한 작업 작성
		}
		
		if (!this->_writeQueue->empty()) {
			// TODO : 예상못한 큐의 제거시 필요한 작업 작성
		}
	}
	inline void push(const T &t)
	{
		SAFE_LOCK(_lock);
		_writeQueue->push(t);
	}
	// 
	inline bool pop(T &t) {
		SAFE_LOCK(_lock);
		size_t size = this->size();
		if (size == 0) {
			return false;
		}
		if (_readQueue->empty()) {
			this->swap();
		}
		t = _readQueue->front();
		_readQueue->pop();
		return true;
		
	}

	inline void swap()
	{
		SAFE_LOCK(_lock);
		if (_writeQueue == &_queue[WRITE_QUEUE]) {
			_writeQueue = &_queue[READ_QUEUE];
			_readQueue = &_queue[WRITE_QUEUE];
		}
		else {
			_writeQueue = &_queue[WRITE_QUEUE];
			_readQueue = &_queue[READ_QUEUE];
		}
	}
	inline bool isEmpty() { 
		return _readQueue->empty(); 
	}
	inline size_t size()
	{
		SAFE_LOCK(_lock);
		size_t size = (size_t)(_queue[WRITE_QUEUE].size() + _queue[READ_QUEUE].size());
		return size;
	}

};