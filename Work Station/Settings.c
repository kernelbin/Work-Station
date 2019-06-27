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

	//设置文件应该不会很大，全都读取出来好了
	PBYTE Buffer;
	DWORD FileSize = 0, FileSizeHigh = 0;
	FileSize = GetFileSize(hFile, &FileSizeHigh);
	if (FileSizeHigh)
	{
		CloseHandle(hFile);
		return SETTINGS_ERROR;
	}
	Buffer = malloc(FileSize + 2);

	DWORD BytesRead;
	ReadFile(hFile, Buffer, FileSize, &BytesRead, 0);
	Buffer[FileSize] = 0;
	Buffer[FileSize + 1] = 0;
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
	//使用默认设置
	lstrcpy(BkgndPicPath, TEXT("C:\\Users\\11603\\Desktop\\1.jpg"));
}

int SaveSettings(BOOL bCreate)
{
	//保存设置
	HANDLE hFile = CreateFile(TEXT("WorkStation.settings"),
		GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, bCreate ? CREATE_ALWAYS : OPEN_EXISTING, 0, 0);

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
	char* SettingsStr = cJSON_Print(Settings);

	DWORD Written;
	WriteFile(hFile, SettingsStr, strlen(SettingsStr), &Written, 0);

	free(SettingsStr);
	CloseHandle(hFile);
	return TRUE;
}
