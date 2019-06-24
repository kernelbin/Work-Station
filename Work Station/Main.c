#include<Windows.h>
#include"EasyWindow.h"
#include"Global.h"
#include<TChar.h>
#include<process.h>
//TYF Cloud 学生版 入口函数

LRESULT CALLBACK ShadowWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MainWndHookProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

HANDLE StartStdoutReceiver(LPSTR lpCmdLine);

void* WINAPI StdoutReceiver();


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	if (lstrlen(lpCmdLine) == 0)
	{
		MainProcessBooter();
	}
	else
	{
		bProgramRunning = TRUE;
		InitConsoleBuffer();
		HANDLE hStdoutRecvThread = StartStdoutReceiver(lpCmdLine);
		if (!hStdoutRecvThread)
		{
			return 0;
		}
		int iRet = MainEntry(hInstance, iCmdShow);
		bProgramRunning = FALSE;
		CancelIoEx(hPipeOutR, 0);
		WaitForSingleObject(hStdoutRecvThread, INFINITE);
		CloseHandle(hStdoutRecvThread);
		return iRet;
	}
}

int MainProcessBooter()//处理妥善Pipe的问题，然后启动新进程
{
	SECURITY_ATTRIBUTES S_Attr;

	S_Attr.nLength = sizeof(SECURITY_ATTRIBUTES);
	S_Attr.bInheritHandle = TRUE;
	S_Attr.lpSecurityDescriptor = NULL;

	hPipeInR = 0, hPipeInW = 0, hPipeOutR = 0, hPipeOutW = 0;

	TCHAR FileName[MAX_PATH];

	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags |= STARTF_USESTDHANDLES;

	BOOL bSuccess = FALSE;

	__try
	{
		if (!CreatePipe(&hPipeInR, &hPipeInW, &S_Attr, 0)) __leave;
		if (!CreatePipe(&hPipeOutR, &hPipeOutW, &S_Attr, 0)) __leave;

		si.hStdInput = hPipeInR;
		si.hStdOutput = hPipeOutW;
		si.hStdError = hPipeOutW;

		if (! GetModuleFileName(0, FileName, MAX_PATH)) __leave;

		TCHAR CommandLine[128];

		_stprintf_s(CommandLine,_countof(CommandLine), TEXT(" %p %p %p %p"), hPipeInR, hPipeInW, hPipeOutR, hPipeOutW);

		if (!CreateProcess(FileName, CommandLine, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi)) __leave;

		bSuccess = TRUE;
	}
	__finally
	{
		if (bSuccess)
		{
			// 貌似没啥好做的，清理退出就行
		}
		else
		{
			ErrorMsgBox(TEXT("标准输入输出重定向出错"));
		}
		
		//执行清理
		if (hPipeInR)
		{
			CloseHandle(hPipeInR);
		}
		if (hPipeInW)
		{
			CloseHandle(hPipeInW);
		}
		if (hPipeOutR)
		{
			CloseHandle(hPipeOutR);
		}
		if (hPipeOutW)
		{
			CloseHandle(hPipeOutW);
		}

		if (pi.hProcess)
		{
			CloseHandle(pi.hProcess);
		}
		if (pi.hThread)
		{
			CloseHandle(pi.hThread);
		}
		

	}
	

	
	
	return 0;
}

int MainEntry(HINSTANCE hInstance, int iCmdShow)
{
	//主程序入口
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
		ShadowWnd[i] = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOOLWINDOW, TEXT("WorkStation ShadowWindow"), TEXT(""), WS_POPUP | WS_VISIBLE, 0, 0, 0, 0, 0, 0, hInstance, i);

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

HANDLE StartStdoutReceiver(LPSTR lpCmdLine)
{

	BOOL bSuccess = FALSE;
	HANDLE StdoutRecvThread = 0;

	hPipeInR = 0, hPipeInW = 0, hPipeOutR = 0, hPipeOutW = 0;
	__try
	{
		if (sscanf_s(lpCmdLine, "%p %p %p %p", &hPipeInR, &hPipeInW, &hPipeOutR, &hPipeOutW) != 4)
		{
			ErrorMsgBox(TEXT("启动参数错误"));
			__leave;
		}

		setvbuf(stdout, 0, _IONBF, 0);

		StdoutRecvThread = _beginthreadex(0, 0, StdoutReceiver, 0, 0, 0);
		

		printf("pwpppwpppw");
		bSuccess = TRUE;
	}
	__finally
	{
	}

	return StdoutRecvThread;
}


void* WINAPI StdoutReceiver()
{
	MessageBox(NULL, TEXT(""), TEXT(""), 0);
	DWORD BufferSize;
	GetNamedPipeInfo(hPipeOutR, NULL, &BufferSize, NULL, NULL);
	char* Buffer;
	
	Buffer = malloc(BufferSize + 1);

	//开始接受
	while (bProgramRunning)
	{
		DWORD BytesRead;
		ReadFile(hPipeOutR, Buffer, BufferSize, &BytesRead, 0);
		Buffer[BytesRead] = 0;
	}

	free(Buffer);

}

typedef unsigned(__stdcall* _beginthreadex_proc_type)(void*);


int ErrorMsgBox(TCHAR Error[])
{
	TCHAR ShowText[256];
	wsprintf(ShowText, TEXT("Work Station在运行过程中出现错误！！\r\n错误信息：%s"), Error);
	return MessageBox(NULL, ShowText, szAppName, MB_ICONERROR);
}



BOOL InitConsoleBuffer()
{
	ConsoleText = InitVText();
	ConsoleInput = InitVText();

	SetVText(ConsoleText, TEXT("Workstation C Shell Console [版本 0.0.1]\n(c)2019 yh。保留所有权利。\n\nC:\\Users\\11603>\n\n"), -1);
	return TRUE;
}