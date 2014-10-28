#pragma once

class Task
{
public:
	virtual ~Task()
	{}

	virtual void Run() = 0;
	virtual void Log() = 0;

	TCHAR *fileName;
	TCHAR *completionReport;
};

