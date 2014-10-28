#pragma once
#include <Windows.h>

class DirectoryLister
{
public:
	DirectoryLister(TCHAR *directoryPath);
	~DirectoryLister();
	TCHAR *GetNextFile();
	bool HasNextFile();
private:
	TCHAR *directoryPath;
	bool hasMoreFiles;
	HANDLE foundItemHandle;
	WIN32_FIND_DATA findData;

	bool FirstLaunched();
	HANDLE GetFirstFileHandle();
	bool IsNotDirectory(WIN32_FIND_DATA findData);
	TCHAR *FormCurrentFilePath();
	void FinishListing();
};

