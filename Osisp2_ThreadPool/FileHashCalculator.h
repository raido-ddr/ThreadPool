#pragma once
#include <Windows.h>

#define BUFFER_SIZE 1024
#define HASH_SIZE 16

class FileHashCalculator
{
public:
	FileHashCalculator();
	~FileHashCalculator();
	TCHAR * CalculateHash(TCHAR *fileName);

private:
	void FinalizeEncryption(HCRYPTHASH hash,
		HCRYPTPROV cryptProvider, HANDLE file);

};

