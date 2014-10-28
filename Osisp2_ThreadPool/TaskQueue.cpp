#include "stdafx.h"
#include "TaskQueue.h"


TaskQueue::TaskQueue()
{
	InitializeCriticalSection(&taskRequestSection);
	InitializeConditionVariable(&tasksAvailableCV);
	shutdownRequestReceived = false;
}


TaskQueue::~TaskQueue()
{
	DeleteCriticalSection(&taskRequestSection);	
}

void TaskQueue::AddTask(Task *task)
{
	EnterCriticalSection(&taskRequestSection);
	tasks.push_back(task);
	LeaveCriticalSection(&taskRequestSection);

	WakeAllConditionVariable(&tasksAvailableCV);
}

Task *TaskQueue::AcquireTask()
{
	EnterCriticalSection(&taskRequestSection);

	while (IsEmpty() && ! shutdownRequestReceived)
	{
		SleepConditionVariableCS(&tasksAvailableCV, &taskRequestSection, INFINITE);
	}		

	if (IsEmpty() && shutdownRequestReceived){
		LeaveCriticalSection(&taskRequestSection);
		return NULL;
	}

	Task *task = tasks.front();
	tasks.pop_front();
	LeaveCriticalSection(&taskRequestSection);

	return task;
}

bool TaskQueue::IsEmpty()
{
	return (tasks.size() == 0);
}

void TaskQueue::Shutdown()
{
	shutdownRequestReceived = true;
	WakeAllConditionVariable(&tasksAvailableCV);
}

