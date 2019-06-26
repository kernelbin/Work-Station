#include<Windows.h>
#include"Settings.h"
#include"Global.h"
#include"cJson.h"

int TryLoadSettings()
{
	HANDLE hFile = CreateFile(TEXT("WorkStation.settings"),
		GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		DWORD Err = GetLastError();
		if (Err == ERROR_FILE_NOT_FOUND)
		{
			return SETTINGS_NOTFOUND;
		}
		if (Err == ERROR_ACCESS_DENIED)
		{
			return SETTINGS_ACCESS_DENIED;
		}
		return SETTINGS_ERROR;
	}

	//�����ļ�Ӧ�ò���ܴ�ȫ����ȡ��������
	PBYTE Buffer;
	DWORD FileSize = 0, FileSizeHigh = 0;
	FileSize = GetFileSize(hFile, &FileSizeHigh);
	if (FileSizeHigh)
	{
		CloseHandle(hFile);
		return SETTINGS_ERROR;
	}
	Buffer = malloc(FileSize);

	DWORD BytesRead;
	ReadFile(hFile, &Buffer, FileSize, &BytesRead, 0);
	if (BytesRead != FileSize)
	{
		free(Buffer);
		CloseHandle(hFile);
		return SETTINGS_ERROR;
	}

	cJSON* Settings = cJSON_Parse(Buffer);

	free(Buffer);
	CloseHandle(hFile);
	return SETTINGS_LOADED;
}

BOOL LoadDefSettings()
{
	//ʹ��Ĭ������
}

int SaveSettings(BOOL bCreate)
{
	//��������
	HANDLE hFile = CreateFile(TEXT("WorkStation.settings"),
		GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, bCreate ? CREATE_ALWAYS: OPEN_EXISTING, 0, 0);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		DWORD Err = GetLastError();
		if (Err == ERROR_FILE_NOT_FOUND)
		{
			return SETTINGS_NOTFOUND;
		}
		if (Err == ERROR_ACCESS_DENIED)
		{
			return SETTINGS_ACCESS_DENIED;
		}
		return SETTINGS_ERROR;
	}


	cJSON* Settings = cJSON_CreateObject();

	DWORD Written;
	WriteFile(hFile, "qwq", 4, &Written, 0);
	CloseHandle(hFile);
	return TRUE;
}
