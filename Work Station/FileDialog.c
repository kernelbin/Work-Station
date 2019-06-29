#include<Windows.h>
#include<commdlg.h>

BOOL OpenFileDialog(HWND hOwner,TCHAR Filter[],TCHAR FileName[],TCHAR FileTitle[])
{
	OPENFILENAME  ofn;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hOwner;
	ofn.hInstance = 0;
	ofn.lpstrFilter = Filter;
	ofn.lpstrCustomFilter = NULL;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = FileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = FileTitle;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = 0;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = NULL;
	ofn.lCustData = 0;
	ofn.lpTemplateName = 0;


	GetOpenFileName(&ofn);
	DWORD err = GetLastError();
	return 0;
}