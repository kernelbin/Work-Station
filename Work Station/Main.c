#include<Windows.h>
#include"EasyWindow.h"
#include"Global.h"
#include<TChar.h>
#include<process.h>
//TYF Cloud ѧ���� ��ں���

LRESULT CALLBACK ShadowWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MainWndHookProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

HANDLE StartStdoutReceiver(LPSTR lpCmdLine);

void* WINAPI StdoutReceiver();

void* WINAPI ConsoleThread();


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	if (lstrlen(lpCmdLine) == 0)
	{
		MainProcessBooter();
		return 0;
	}
	else
	{
		bProgramRunning = TRUE;
		if (!InitConsoleBuffer())
		{
			ErrorMsgBox(TEXT("��ʼ�������л�����ʧ��"));
			return 0;
		}
		
		int iRet = MainEntry(hInstance,lpCmdLine, iCmdShow);
		bProgramRunning = FALSE;
		CancelIoEx(hPipeOutR, 0);
		WaitForSingleObject(hStdoutRecvThread, INFINITE);
		CloseHandle(hStdoutRecvThread);
		return iRet;
	}
}

int MainProcessBooter()//��������Pipe�����⣬Ȼ�������½���
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

		if (!GetModuleFileName(0, FileName, MAX_PATH)) __leave;

		TCHAR CommandLine[128];

		_stprintf_s(CommandLine, _countof(CommandLine), TEXT(" %p %p %p %p"), hPipeInR, hPipeInW, hPipeOutR, hPipeOutW);

		if (!CreateProcess(FileName, CommandLine, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi)) __leave;

		bSuccess = TRUE;
	}
	__finally
	{
		if (bSuccess)
		{
			// ò��ûɶ�����ģ������˳�����
		}
		else
		{
			ErrorMsgBox(TEXT("��׼��������ض������"));
		}

		//ִ������
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

int MainEntry(HINSTANCE hInstance,LPSTR lpCmdLine, int iCmdShow)
{
	//���������
	WNDCLASS wndclass;
	//ע�ᴰ����
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
		MessageBoxW(0, TEXT("ע�ᴰ����ʧ��"), szAppName, 0);
		return 0;
	}

	if (!InitEZWindow())
	{
		MessageBox(0, TEXT("��ʼ��EasyWindowsʧ��"), szAppName, 0);
		return 0;
	}

	MainWnd = NULL;


	//������Ӱ����

	for (int i = 0; i < 8; i++)
	{
		ShadowWnd[i] = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOOLWINDOW, TEXT("WorkStation ShadowWindow"), TEXT(""), WS_POPUP | WS_VISIBLE, 0, 0, 0, 0, 0, 0, hInstance, i);

	}

	MainWnd = CreateMainParentWindow(
		(GetSystemMetrics(SM_CXSCREEN) - DEF_WIDTH) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - DEF_HEIGHT) / 2,
		DEF_WIDTH, DEF_HEIGHT, iCmdShow, MainWndProc);

	//��װ����
	OldMainWndProc = SetWindowLongPtr(MainWnd->hParent, GWLP_WNDPROC, MainWndHookProc);
	MainWndHookProc(MainWnd->hParent, WM_CREATE, 0, 0);


	hStdoutRecvThread = StartStdoutReceiver(lpCmdLine);
	if (!hStdoutRecvThread)
	{
		ErrorMsgBox(TEXT("��ʼ����׼��������ض�������߳�ʧ��"));
		return 0;
	}

	//����һ���߳����������̨
	_beginthreadex(0, 0, ConsoleThread, 0, 0, 0);

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
			ErrorMsgBox(TEXT("������������"));
			__leave;
		}

		setvbuf(stdout, 0, _IONBF, 0);

		StdoutRecvThread = _beginthreadex(0, 0, StdoutReceiver, 0, 0, 0);


		//printf("qwqqwqwqwqwq QWQWQWQ����");
		bSuccess = TRUE;
	}
	__finally
	{
	}

	return StdoutRecvThread;
}


void* WINAPI StdoutReceiver()
{

	//MessageBox(NULL, TEXT(""), TEXT(""), 0);
	DWORD BufferSize;
	GetNamedPipeInfo(hPipeOutR, NULL, &BufferSize, NULL, NULL);
	char* Buffer;

	Buffer = malloc(BufferSize + 1);

	if (!Buffer)
	{
		ErrorMsgBox(TEXT("�����ڴ�ʧ��"));
		ExitProcess(0);//�ⲻ�����̣߳����Ա����˳�
		//TODO: ò�������û����԰�����Ի����������ﲢ�Ҽ�����������棿
		return 0;
	}
	else
	{
		//��ʼ����
		while (bProgramRunning)
		{
			DWORD BytesRead;
			DWORD TotalAvail;
			PeekNamedPipe(hPipeOutR, Buffer, BufferSize, &BytesRead, &TotalAvail, 0);
			if (TotalAvail == 0)
			{
				ReadFile(hPipeOutR, Buffer, 0, &BytesRead, 0);
				PeekNamedPipe(hPipeOutR, Buffer, BufferSize, &BytesRead, &TotalAvail, 0);
			}
			if (TotalAvail > BufferSize)
			{
				//wtf?
				ErrorMsgBox(TEXT("�ܵ���ʣ�����ݹ���"));
				ExitProcess(0);//�ⲻ�����̣߳����Ա����˳�
			}
			

			//�������ʽ��ȡ�ļ�֮ǰ������ʵ�Ѿ�peek�����ˣ��ֹ����unicode
			BOOL bRet = ReadFile(hPipeOutR, Buffer, TotalAvail, &BytesRead, 0);
			
			if ((!bRet) && (bProgramRunning))
			{
				ErrorMsgBox(TEXT("IO�ض��� ReadFile ��������ʧ��"));
				ExitProcess(0);//�ⲻ�����̣߳����Ա����˳�
				//TODO: ò�������û����԰�����Ի����������ﲢ�Ҽ�����������棿
				return 0;
			}
			Buffer[TotalAvail] = 0;
			//ת���ɿ��ַ���Ȼ����Ϣ

			int cchLen = MultiByteToWideChar(CP_ACP, 0, Buffer, TotalAvail, 0, 0);

			WCHAR * wcBuf = malloc((cchLen + 1) * sizeof(WCHAR));

			MultiByteToWideChar(CP_ACP, 0, Buffer, TotalAvail, wcBuf, (cchLen + 1));
			wcBuf[cchLen] = 0;
			//����Ϣ
			SendMessage(MainWnd->hParent, WM_STDIO_REDIRECT, wcBuf, cchLen);

			/*TCHAR OutShow[100];
			wsprintf(OutShow, TEXT("%d"), TotalAvail);
			MessageBox(NULL, OutShow, TEXT("qwq123456789"), 0);*/
		}
	}


	free(Buffer);

}

int ErrorMsgBox(TCHAR Error[])
{
	TCHAR ShowText[256];
	wsprintf(ShowText, TEXT("Work Station�����й����г��ִ��󣡣�\r\n������Ϣ��%s"), Error);
	return MessageBox(NULL, ShowText, szAppName, MB_ICONERROR);
}



BOOL InitConsoleBuffer()
{
	if (!(ConsoleText = InitVText()))return FALSE;
	if (!(ConsoleInput = InitVText()))return FALSE;

	return TRUE;
}



void* WINAPI ConsoleThread()
{
	int a;
	while (1)
	{
		printf("��������������������������������������and english");
		Sleep(10);
	}
	
	return 0;
}

int checkprime(int a)
{
	int isprime = 1;
	for (int i = 2; i < a; i++)
	{
		if (a % i == 0)
		{
			isprime = 0;
			break;
		}
	}
	return isprime;
}