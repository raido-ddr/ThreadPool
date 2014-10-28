#pragma once
#include <list>
#include <Windows.h>
#include "Task.h"

using namespace std;

class TaskQueue
{
public:
	TaskQueue();
	~TaskQueue();
	void AddTask(Task *task);
	Task *AcquireTask();
	bool IsEmpty();
	void Shutdown();

private:
	list<Task *> tasks;
	CRITICAL_SECTION taskRequestSection;
	CONDITION_VARIABLE tasksAvailableCV;
	bool shutdownRequestReceived;
};

