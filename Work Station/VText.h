#pragma once
#include<Windows.h>


typedef struct __tagVectorText
{
	int Capibility;
	int Length;
	TCHAR* Text;
}VECTORTEXT, VTEXT, * pVTEXT;


pVTEXT InitVText();
BOOL FreeVText(pVTEXT pVText);
BOOL ClearVText(pVTEXT pVText);
BOOL SetVText(pVTEXT pVText, TCHAR Str[], int len);
