#pragma once
#include<Windows.h>


typedef struct __tagVectorTextA
{
	int Capibility;
	int Length;
	char* Text;
}VECTORTEXTA, VTEXTA, * pVTEXTA;


typedef struct __tagVectorTextW
{
	int Capibility;
	int Length;
	WCHAR* Text;
}VECTORTEXTW, VTEXTW, * pVTEXTW;



#ifdef UNICODE
typedef VECTORTEXTW VECTORTEXT;
typedef VECTORTEXTW VTEXT;
typedef VECTORTEXTW *pVTEXT;
#define SetVText SetVTextW
#define CatVText CatVTextW
#define VTextInsertChar VTextInsertCharW
#define VTextDeleteChar VTextDeleteCharW
#else
typedef VECTORTEXTA VECTORTEXT;
typedef VECTORTEXTA VTEXT;
typedef VECTORTEXTA pVTEXT;
#define SetVText SetVTextA
#define CatVText CatVTextA
#define VTextInsertChar VTextInsertCharA
#define VTextDeleteChar VTextDeleteCharA
#endif
//
//
//typedef struct __tagVectorText
//{
//	int Capibility;
//	int Length;
//	TCHAR* Text;
//}VECTORTEXT, VTEXT, * pVTEXT;


pVTEXT InitVText();
BOOL FreeVText(pVTEXT pVText);
BOOL ClearVText(pVTEXT pVText);

BOOL VTextInsertCharA(pVTEXTA pVText, char ch, int InsertBefore);
BOOL VTextInsertCharW(pVTEXTW pVText, WCHAR ch, int InsertBefore);
BOOL VTextDeleteCharA(pVTEXTA pVText, int DeleteBefore);
BOOL VTextDeleteCharW(pVTEXTW pVText, int DeleteBefore);

BOOL SetVTextA(pVTEXTA pVText, char Str[], int len);
BOOL SetVTextW(pVTEXTW pVText, WCHAR Str[], int len);

BOOL CatVTextA(pVTEXTA pVTextA, pVTEXTA pVTextB);
BOOL CatVTextW(pVTEXTW pVTextA, pVTEXTW pVTextB);