#include<Windows.h>
#include"EasyWindow.h"
#include"Global.h"
//TYF Cloud 学生版 入口函数

LRESULT CALLBACK ShadowWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MainWndHookProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	WNDCLASS wndclass;
	//注册窗口类
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = ShadowWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = sizeof(LONG_PTR);
	wndclass.hInstance = GetModuleHandle(NULL);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("WorkStation ShadowWindow");
	if (!RegisterClass(&wndclass))
	{
		MessageBoxW(0, TEXT("注册窗口类失败"), szAppName, 0);
		return 0;
	}

	if (!InitEZWindow())
	{
		MessageBox(0, TEXT("初始化EasyWindows失败"), szAppName, 0);
		return 0;
	}

	MainWnd = NULL;
	
	
	//创建阴影窗口

	for (int i = 0; i < 8; i++)
	{
		ShadowWnd[i] = CreateWindowEx(WS_EX_LAYERED|WS_EX_TOOLWINDOW, TEXT("WorkStation ShadowWindow"), TEXT(""), WS_POPUP | WS_VISIBLE, 0, 0, 0, 0, 0, 0, hInstance, i);
	}

	MainWnd = CreateMainParentWindow(
		(GetSystemMetrics(SM_CXSCREEN) - DEF_WIDTH) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - DEF_HEIGHT) / 2,
		DEF_WIDTH, DEF_HEIGHT, iCmdShow, MainWndProc);

	//安装钩子
	OldMainWndProc = SetWindowLongPtr(MainWnd->hParent, GWLP_WNDPROC, MainWndHookProc);
	MainWndHookProc(MainWnd->hParent, WM_CREATE, 0, 0);

	return EZWndMessageLoop();
}