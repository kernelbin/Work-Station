#pragma once
#include"EZWndProc.h"
#include"LayoutDefine.h"

//应用程序名称定义

#define szAppName TEXT("YH's Work Station")

//窗口句柄
EZWND MainWnd, LBorder, PageHolder, ScrollBar;
HWND ShadowWnd[8];


//消息钩子
WNDPROC OldMainWndProc;





EZWND CreateMainParentWindow(int x, int y, int Width, int Height, int iCmdShow, EZWNDPROC ezWndProc);


//界面颜色定义
COLORREF BackgroundColor;//背景颜色
COLORREF LightBackgroundColor;//浅背景颜色
COLORREF AppColor;

//界面字体定义
TCHAR FontFaceName[32];

//GDI对象管理相关API
int InitGDIObject();
HFONT CreateAppFont(int Height, int Weight);

#define ANIMATION_TIMESPACE 32


int AddPage(EZWNDPROC ezWndProc);//添加一个新的页。返回页编号，失败返回 -1
int RemovePage(int PageID);
int SelectPage(int PageID);
int SetPageTransparent(BOOL bTrans);