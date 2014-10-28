#pragma once
#include "Thread.h"
#include "Task.h"
#include "TaskQueue.h"
#include <list>

//using namespace std;

class ThreadPool
{
public:
	ThreadPool(int threadCount);
	~ThreadPool();
	void ExecuteTask(Task *task);
	Task *AcquireScheduledTask();
	void Shutdown();
	

private:
	Thread *threads;
	int threadCount;
	Thread **workerThreads;
	TaskQueue tasks;
	
};

