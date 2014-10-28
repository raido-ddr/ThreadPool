#include "stdafx.h"
#include "FileHashTask.h"
#include "FileHashCalculator.h"
#include <Windows.h>
#pragma warning(disable: 4996)


FileHashTask::FileHashTask(TCHAR *fileName)
{
	this->fileName = new TCHAR[_MAX_PATH];
	wcscpy(this->fileName, fileName);
}


FileHashTask::~FileHashTask()
{
	if (fileName != NULL)
	{
		delete fileName;
	}

	if (hashSum != NULL)
	{
		delete hashSum;
	}
}


void FileHashTask::Run()
{
	FileHashCalculator *calculator = new FileHashCalculator();
	hashSum = calculator->CalculateHash(fileName);
}

void FileHashTask::Log()
{
	wprintf(L"File: %s; Hash: %s %d\n", fileName, hashSum, GetCurrentThreadId());
}