#include"stdafx.h"
#include "Task.h"

TaskNode::TaskNode(Work * workObject,int freqSec, int duraSec)
{
	_workobject = workObject;
	_freqencySec = freqSec;
	_durationSec = duraSec;
	this->nextTick();
}

TaskNode::~TaskNode()
{
	SAFE_DELETE(_workobject);
}

void TaskNode::nextTick()
{
	_nextTick = NOW_TICK() + (tick_t)_freqencySec;
}

bool TaskNode::isExpired()
{
	// 
	if (_workobject == nullptr) {
		return true;
	}
	if (_durationSec != TICK_INFINTY) {
		// 지속적으로 Task가 아닌경우
		if (_durationSec < NOW_TICK()) {
			// Task의 총 시간이 끝났을경우
			return true;
		}
	}
	return false;
}

void TaskNode::tick()
{
	if (_nextTick < NOW_TICK()) {
		// TODO : PostQueuedCompletionStatus 사용하여 Contentsprocess에 전달하여
		// logic 수행
		this->nextTick();
	}
}

TaskManager::TaskManager() : _lock(L"TaskManager")
{
	this->run();
}

TaskManager::~TaskManager()
{
	TaskNode * deleNode;
	while (!_taskqueue.empty()) {
		deleNode = _taskqueue.top();
		_taskqueue.pop();
		SAFE_DELETE(deleNode);
	}
}

void TaskManager::add(TaskNode * taskNode)
{
	_lock.lock(_W(__FILE__), __LINE__);
	_taskqueue.push(taskNode);
	_lock.Unlock();
}

void TaskManager::process()
{
	while (!_shutdown) {
		CONTEXT_SWITCH;
		_lock.lock(_W(__FILE__), __LINE__);
		while(!_taskqueue.empty()) {
			if (_taskqueue.top()->isExpired()) 
			{
				// 만료 Task 삭제
				TaskNode * delNode = _taskqueue.top();
				_taskqueue.pop();
				SAFE_DELETE(delNode);
				continue;
			}
			TaskNode * addNode = _taskqueue.top();
			_taskqueue.pop();
			addNode->tick();
			_taskqueue.push(addNode);
			break;
		}
		_lock.Unlock();
	}
}

void TaskManager::run()
{
	_thread = MAKE_THREAD(TaskManager, process);
	printf("** Task Thread Create \n");
}
