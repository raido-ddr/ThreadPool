#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>
#include "FileHashCalculator.h"
#pragma warning(disable: 4996)


FileHashCalculator::FileHashCalculator()
{
}


FileHashCalculator::~FileHashCalculator()
{
}

TCHAR * FileHashCalculator::CalculateHash(TCHAR *fileName)
{
	DWORD dwStatus = 0;
	BOOL bResult = FALSE;
	HCRYPTPROV cryptProvider = 0;
	HCRYPTHASH hash = 0;
	HANDLE hFile = NULL;
	BYTE rgbFile[BUFFER_SIZE];
	DWORD cbRead = 0;
	BYTE rgbHash[HASH_SIZE];
	DWORD cbHash = 0;
	const TCHAR rgbDigits[] = L"0123456789abcdef";
	TCHAR *hashSum;

	hFile = CreateFile(fileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		dwStatus = GetLastError();
		printf("Failed to file %s\nError: %d\n", fileName,
			dwStatus);
		return NULL;
	}

	if (!CryptAcquireContext(&cryptProvider, NULL, NULL,
		PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
	{
		dwStatus = GetLastError();
		printf("CryptAcquireContext failed: %d\n", dwStatus);
		CloseHandle(hFile);
		return NULL;
	}

	if (!CryptCreateHash(cryptProvider, CALG_MD5, 0, 0, &hash))
	{
		dwStatus = GetLastError();
		printf("CryptAcquireContext failed: %d\n", dwStatus);
		CloseHandle(hFile);
		CryptReleaseContext(cryptProvider, 0);
		return NULL;
	}

	while (bResult = ReadFile(hFile, rgbFile, BUFFER_SIZE,
		&cbRead, NULL))
	{
		if (cbRead == 0)
		{
			break;
		}

		if (!CryptHashData(hash, rgbFile, cbRead, 0))
		{
			dwStatus = GetLastError();
			printf("Encryption error: %d\n", dwStatus);
			FinalizeEncryption(hash, cryptProvider, hFile);
			return NULL;
		}
	}

	if (!bResult)
	{
		dwStatus = GetLastError();
		printf("Failed to read file: %d\n", dwStatus);
		FinalizeEncryption(hash, cryptProvider, hFile);
		return NULL;
	}

	cbHash = HASH_SIZE;
	if (CryptGetHashParam(hash, HP_HASHVAL, rgbHash, &cbHash, 0))
	{
		int hashSize = cbHash * 2  + 2;
		hashSum = (TCHAR*) (calloc(hashSize, sizeof(TCHAR)));

		for (DWORD i = 0; i < cbHash; i++)
		{
			hashSum[2 * i] = rgbDigits[rgbHash[i] >> 4];
			hashSum[2 * i + 1] = rgbDigits[rgbHash[i] & 0xf];
		}		

		_tcscat(hashSum, L"\0");
	}
	else
	{
		dwStatus = GetLastError();
		printf("Failed to get hash parameters: %d\n", dwStatus);
	}

	FinalizeEncryption(hash, cryptProvider, hFile);
	return hashSum;
}

void FileHashCalculator::FinalizeEncryption(HCRYPTHASH hash, 
	HCRYPTPROV cryptProvider, HANDLE file)
{
	CryptDestroyHash(hash);
	CryptReleaseContext(cryptProvider, 0);
	CloseHandle(file);
}
