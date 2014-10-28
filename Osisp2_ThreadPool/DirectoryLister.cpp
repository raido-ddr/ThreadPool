#include "stdafx.h"
#include <stdexcept>
#include "DirectoryLister.h"
#include <Windows.h>
#include "Shlwapi.h"
#pragma warning(disable: 4996)
#pragma comment(lib, "Shlwapi.lib")


DirectoryLister::DirectoryLister(TCHAR *directoryPath)
{
	if (!PathFileExists(directoryPath))
	{
		throw std::exception("Requested directory does not exist.");
	}

	this->directoryPath = directoryPath;
	this->foundItemHandle = NULL;
	this->hasMoreFiles = true;
}


DirectoryLister::~DirectoryLister()
{
	if (directoryPath != NULL)
	{
		delete directoryPath;
	}
}

TCHAR *DirectoryLister::GetNextFile()
{
	while (hasMoreFiles)
	{
		if (FirstLaunched())
		{			
			foundItemHandle = GetFirstFileHandle();
		}
		else
		{
			if (FindNextFile(foundItemHandle, &findData) == 0)
			{
				FinishListing();				
			}
				
		}

		if (IsNotDirectory(findData))
		{
			break;
		}			
	}

	if (! hasMoreFiles)
	{
		return NULL;
	}

	return FormCurrentFilePath();
}

bool DirectoryLister::HasNextFile()
{
	return hasMoreFiles;
}

bool DirectoryLister::FirstLaunched()
{
	return (foundItemHandle == NULL);
}

HANDLE DirectoryLister::GetFirstFileHandle()
{
	int fullPathSize = (wcslen(directoryPath) + 2) * sizeof(TCHAR);
	TCHAR* fullPath = new TCHAR[fullPathSize];
	fullPath = wcscpy(fullPath, directoryPath);
	fullPath = wcscat(fullPath, L"\\*.*");
	return FindFirstFile(fullPath, &findData);
}

bool DirectoryLister::IsNotDirectory(WIN32_FIND_DATA findData)
{
	return !(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

TCHAR * DirectoryLister::FormCurrentFilePath()
{
	int filePathSize = wcslen(directoryPath) + wcslen(findData.cFileName) + 3;
	TCHAR *fullFilePath = new TCHAR[filePathSize];
	wcscpy(fullFilePath, directoryPath);
	wcscat(fullFilePath, L"\\");
	wcscat(fullFilePath, findData.cFileName);
	wcscat(fullFilePath, L"\0");

	return fullFilePath;
}

void DirectoryLister::FinishListing()
{
	hasMoreFiles = false;
	FindClose(foundItemHandle);
}