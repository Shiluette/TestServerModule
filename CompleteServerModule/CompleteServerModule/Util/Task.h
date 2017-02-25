#pragma once
#include"stdafx.h"

#define TICK_INFINTY 0 
class Work;
class TaskNode
{
	Work * _workobject;
	int _freqencySec;
	int _durationSec;

public:
	tick_t _nextTick;

	TaskNode(Work * workObject , int freqSec, int duraSec);
	~TaskNode();

	void nextTick();
	bool isExpired();

	void tick();
};
class mycomp
{
public:
	bool operator() (const TaskNode * lhs, const TaskNode * rhs) const
	{
		return (lhs->_nextTick > rhs->_nextTick);
	}
};

class TaskManager : public Singleton<TaskManager>
{
	priority_queue<TaskNode *, vector<TaskNode *>, mycomp> _taskqueue;
	Thread * _thread;
	Lock _lock;
	int _id;
public:
	TaskManager();
	~TaskManager();
	void add(TaskNode * taskNode);

	void process();
	void run();
};
