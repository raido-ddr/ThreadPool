#pragma once
#include "Task.h"

#define HASH_SUM_LENGTH 33

class FileHashTask : public Task
{
public:
	//TCHAR *fileName;
	FileHashTask(TCHAR *fileName);
	~FileHashTask();
	virtual void Run() override;
	virtual void Log() override;

private:	
	TCHAR *hashSum;

};

