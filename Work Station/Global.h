#pragma once
#include"EZWndProc.h"
#include"LayoutDefine.h"
#include"VText.h"
#include"Settings.h"

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
LOGFONT FontForm;


//GDI对象管理相关API
int InitGDIObject();
HFONT CreateAppFont(int Height, int Weight);

//动画刷新频率
#define ANIMATION_TIMESPACE 32

//页管理
int AddPage(EZWNDPROC ezWndProc);//添加一个新的页。返回页编号，失败返回 -1
int RemovePage(int PageID);
int SelectPage(int PageID);
int SetPageTransparent(BOOL bTrans);




//管道以及标准输入输出流
HANDLE hPipeInR, hPipeInW, hPipeOutR, hPipeOutW;
HANDLE hStdoutRecvThread;


//指示主程序仍然在运行的变量
BOOL bProgramRunning;


//开始编译事件对象
HANDLE hEventStartCompile;
BOOL IsCommandRunning;

//执行代码缓冲区
char *CodeBuffer;

//控制台文本缓冲区
pVTEXT ConsoleText, ConsoleInput;


//自定义Windows消息
#define WM_STDIO_REDIRECT (WM_USER + 1)




BOOL InitGdiPlus();
BOOL CleanGdiPlus();

HBITMAP LoadPicFromFile(TCHAR Path[]);



//全局设置
TCHAR BkgndPicPath[MAX_PATH];


BOOL OpenFileDialog(HWND hOwner, TCHAR Filter[], TCHAR FileName[], TCHAR FileTitle[]);