#include "stdafx.h"
#include "Thread.h"
#include "ThreadPool.h"


Thread::Thread(ThreadPool *pool)
{
	this->pool = pool;
	this->keepRunning = true;
	threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc,
		(LPVOID)this, 0, &threadId);
}

Thread::~Thread()
{
	CloseHandle(threadHandle);
}

DWORD WINAPI Thread::ThreadProc(LPVOID param)
{
	Thread *self = (Thread*) param;

	while (self->keepRunning)
	{		
		Task *task = self->pool->AcquireScheduledTask();
		if (task != NULL)
		{
			task->Run();	
			task->Log();
			delete task;
		}
		else
		{
			delete task;
			break;
		}			

	}

	return true;
}


void Thread::WaitForThreadToExit()
{
	WaitForSingleObject(threadHandle, INFINITE);
}