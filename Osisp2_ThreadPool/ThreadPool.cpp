#include "stdafx.h"
#include "ThreadPool.h"
//#include "Thread.h"


ThreadPool::ThreadPool(int threadCount)
{
	this->threadCount = threadCount;
	workerThreads = new Thread* [threadCount];

	for (int i = 0; i < threadCount; i++)
	{
		workerThreads[i] = new Thread(this);
	}
}


ThreadPool::~ThreadPool()
{
	for (int i = 0; i < threadCount; i++)
	{
		delete workerThreads[i];
	}

	delete workerThreads;
}



void ThreadPool::ExecuteTask(Task *task)
{
	tasks.AddTask(task);
}

void ThreadPool::Shutdown()
{
	tasks.Shutdown();
	for (int i = 0; i < threadCount; i++)
	{
		workerThreads[i]->WaitForThreadToExit();
	}
}

Task * ThreadPool::AcquireScheduledTask()
{
	return tasks.AcquireTask();
}

