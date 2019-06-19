#include<Windows.h>
#include"EasyWindow.h"
#include"Global.h"

//EZWNDPROC MainPageProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
//{
//	static EZWND ScrollBar, ScrollLayer;
//	
//	switch (message)
//	{
//	case EZWM_CREATE:
//		//ScrollBar = CreateEZStyleWindow(ezWnd, TEXT(""), EZS_CHILD | EZS_CHILD_VSCROLL, ezWnd->Width - 18, 0, 18, ezWnd->Height);
//		//ScrollLayer = CreateEZWindow(ezWnd, 0, 0, 0, 0, MainPageContentScrollProc);
//
//
//		return 0;
//
//	case EZWM_SIZE:
//		MoveEZWindow(ScrollBar, ezWnd->Width - 18, 0, 18, ezWnd->Height, 0);
//		MoveEZWindow(ScrollLayer, 0, 0, ezWnd->Width - 18, ezWnd->Height, 0);
//		EZSendMessage(ScrollBar, EZWM_SETSCROLLPOS, 0, ezWnd->Height);
//		return 0;
//
//	case EZWM_USER_NOTIFY:
//		EZSendMessage(ScrollBar, EZWM_SETSCROLLRANGE, lParam, 0);
//		return 0;
//
//
//	case EZWM_MOUSEMOVE:
//		Sleep(0);
//		return 0;
//	case EZWM_SCROLLPOSCHANGE:
//		ScrollEZWindow(ScrollLayer, 0, -(int)wParam, 0);
//		return 0;
//	}
//	return 0;
//}

//EZWNDPROC MainPageProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
//{
//	static EZWND ContentWnd;
//	switch (message)
//	{
//	case EZWM_CREATE:
//		ContentWnd = CreateEZWindow(ezWnd, 0, 0, 0, 0, MainPageContentProc);
//		return 0;
//	case EZWM_SIZE:
//		//MoveEZWindow(ContentWnd, 0, 0, ezWnd->Width, ezWnd->Height, 0);
//		//������Ϣ֪ͨ��ȣ����Ӵ����Լ������߶ȣ�Ȼ��������
//	{
//		int iHeight = max(EZSendMessage(ContentWnd, EZWM_USER_NOTIFY, 0, ezWnd->Width), ezWnd->Height);
//
//		
//		MoveEZWindow(ContentWnd, 0, 0, ezWnd->Width, iHeight, 0);
//		EZSendMessage(ScrollBar, EZWM_SETSCROLLRANGE, iHeight, 0);
//	//	EZSendMessage(ezWnd->ezParent, EZWM_USER_NOTIFY, 0, iHeight);
//		return 0;
//	}
//	}
//	return 0;
//}

EZWNDPROC MainPageProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	static EZWND PluginList;
	switch (message)
	{
	case EZWM_CREATE:
		PluginList = CreateEZWindow(ezWnd, 0, 0, 0, 0, PluginListProc);
		//�������Ϣ����ʲô�Ĵ��ڹ��̶�д�ã�Ȼ�����������EZWM_SIZE
		return 0;

	case EZWM_SIZE:
		//���ݽ����Ⱦ���UI����
		if (ezWnd->Width < UI_BREAKPOINT)
		{
			//С�Ž���
		}
		else
		{
			//��Ž���
		}
		return 0;

	case EZWM_USER_NOTIFY:
		//�Ű�Ԫ�ز��֣�����߶ȣ����е����߶ȣ������ء�

		//��ʱ�̶�500��debug��
		
		return 500;
	case EZWM_DRAW:
		//Ϲ����
		MoveToEx(wParam, 10, 10, 0);
		LineTo(wParam, 10, 200);
		return 0;
	}
	return 0;
}

EZWNDPROC PluginListProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case EZWM_DRAW:
		PatBlt(wParam, 0, 0, ezWnd->Width, ezWnd->Height, WHITENESS);
		return 0;
	}
}