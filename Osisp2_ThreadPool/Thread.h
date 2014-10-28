#pragma once
#include "Windows.h"
#include "Task.h"
//#include "ThreadPool.h"

class ThreadPool;

class Thread
{
public:
	Thread(ThreadPool *pool);
	~Thread();
	static DWORD WINAPI ThreadProc(LPVOID param);
	void WaitForThreadToExit();


private:
	HANDLE threadHandle;
	DWORD threadId;
	//static bool keepRunning;
	bool keepRunning;
	ThreadPool *pool;
	Task *task;

};


