#pragma once
#include"EZWndProc.h"
#include"LayoutDefine.h"

//Ӧ�ó������ƶ���

#define szAppName TEXT("YH's Work Station")

//���ھ��
EZWND MainWnd, LBorder, PageHolder, ScrollBar;
HWND ShadowWnd[8];


//��Ϣ����
WNDPROC OldMainWndProc;





EZWND CreateMainParentWindow(int x, int y, int Width, int Height, int iCmdShow, EZWNDPROC ezWndProc);


//������ɫ����
COLORREF BackgroundColor;//������ɫ
COLORREF LightBackgroundColor;//ǳ������ɫ
COLORREF AppColor;

//�������嶨��
TCHAR FontFaceName[32];

//GDI����������API
int InitGDIObject();
HFONT CreateAppFont(int Height, int Weight);

#define ANIMATION_TIMESPACE 32


int AddPage(EZWNDPROC ezWndProc);//���һ���µ�ҳ������ҳ��ţ�ʧ�ܷ��� -1
int RemovePage(int PageID);
int SelectPage(int PageID);
int SetPageTransparent(BOOL bTrans);