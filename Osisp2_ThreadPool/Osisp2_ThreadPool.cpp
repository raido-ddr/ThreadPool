// Osisp2_ThreadPool.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Thread.h"
#include "ThreadPool.h"
#include "FileHashTask.h"
#include "DirectoryLister.h"
#pragma warning(disable: 4996)

void SetUpConsole();
TCHAR *ReadTargetDirectory();
int ReadThreadCount();

int _tmain(int argc, _TCHAR* argv[])
{
	SetUpConsole();

	TCHAR *targetDirecotry = ReadTargetDirectory();
	int threadCount = ReadThreadCount();

	ThreadPool *pool = new ThreadPool(threadCount);

	DirectoryLister *lister;
	try {
		lister = new DirectoryLister(targetDirecotry);
	}
	catch (std::exception &ex) {
		printf("\n%s\n", ex.what());
		system("PAUSE");
		return 0;
	}	
	
	do {
		TCHAR *nextFilePath = lister->GetNextFile();
		if (lister->HasNextFile())
		{
			pool->ExecuteTask(new FileHashTask(nextFilePath));
		}
		
	} while (lister->HasNextFile());

	pool->Shutdown();

	delete pool;
	
	printf("All tasks have been accomplished.\n");
	system("PAUSE");
}

void SetUpConsole()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");
}


TCHAR *ReadTargetDirectory()
{
	TCHAR *targetDirecotry = new TCHAR[_MAX_PATH];
	wprintf(L"Enter target directory: \n");
	_tscanf(L"%s", targetDirecotry);

	return targetDirecotry;
}

int ReadThreadCount()
{
	int threadCount = 0;

	do {
		wprintf(L"Enter pool capacity (threads count): \n");
		_tscanf(L"%d", &threadCount);
	} while (threadCount <= 0);

	return threadCount;
}