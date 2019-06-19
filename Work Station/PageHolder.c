#include<Windows.h>
#include"EasyWindow.h"
#include"Global.h"

#define MAX_PAGE 16



int iSelect = -1;

EZWND PageList[MAX_PAGE] = { 0 };


int AddPage(EZWNDPROC ezWndProc)//���һ���µ�ҳ������ҳ��ţ�ʧ�ܷ��� -1
{
	int PageID;
	for (PageID = 0; PageID < MAX_PAGE; PageID++)
	{
		if (PageList[PageID] == 0)
		{
			break;
		}
	}
	

	if (PageID == MAX_PAGE)
	{
		//����
		return -1;
	}
	
	PageList[PageID] = CreateEZWindow(PageHolder, 0, 0, PageHolder->Width, PageHolder->Height, ezWndProc);

	PageList[PageID]->MouseMsgRecv = 2;
	PageList[PageID]->ShowState = 2;

	return PageID;
}

int RemovePage(int PageID)
{
	SelectPage(-1);
	DestroyEZWindow(PageList[PageID]);

	return 0;
}

int SelectPage(int PageID)
{
	//����ԭ��ѡ���ҳ��
	if (iSelect != -1)
	{
		PageList[iSelect]->MouseMsgRecv = 2;
		PageList[iSelect]->ShowState = 2;
	}

	iSelect = PageID;

	//��������ʾѡ��ҳ��
	if (iSelect != -1 && PageList[iSelect])
	{
		//����߶�
		int iHeight = EZSendMessage(PageList[iSelect], EZWM_USER_NOTIFY, 0, PageHolder->Width);
		MoveEZWindow(PageList[iSelect], 0, 0, PageHolder->Width, iHeight, 0);
		EZSendMessage(ScrollBar, EZWM_SETSCROLLRANGE, max(iHeight,PageHolder->Height), 0);
		
		
		PageList[iSelect]->MouseMsgRecv = 1;
		PageList[iSelect]->ShowState = 1;
	}
	EZRepaint(MainWnd, 0);

	//���Ŀǰ�������ǹ���ģ������л�ҳ֮����Ҫ���ù�����
	
	return 0;
}

int SetPageTransparent(BOOL bTrans)
{
	PageHolder->MouseMsgRecv = bTrans ? 2 : 1;	
}

EZWNDPROC PageHolderProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	static int Transparent;
	static HDC hdcBlack;
	switch (message)
	{
	case EZWM_CREATE:
		hdcBlack = GetMemDC(ezWnd->hdc, 1, 1);
		Transparent = 0;
		return 0;
	case EZWM_SIZE:
		AdjustMemDC(hdcBlack, ezWnd->hdc, ezWnd->Width, ezWnd->Height);
		PatBlt(hdcBlack, 0, 0, ezWnd->Width, ezWnd->Height, BLACKNESS);

		//�ƶ�ҳ���ڣ�ֻ��������ʾ������������ĵ���Ҫ��ʾ��ʱ���ٵ���
		
		if (iSelect != -1)
		{
			//������Ϣ��֪ͨ��ȣ����Ӵ��ڼ���ø߶�Ȼ�󷵻أ�Ȼ����ݷ���ֵ�������ڴ�С
			int iHeight = EZSendMessage(PageList[iSelect], EZWM_USER_NOTIFY, 0, ezWnd->Width);
			MoveEZWindow(PageList[iSelect], 0, 0, ezWnd->Width, iHeight, 0);
			EZSendMessage(ScrollBar, EZWM_SETSCROLLRANGE, max(iHeight,ezWnd->Height), 0);
		}
		
		return 0;

	case EZWM_LBUTTONDOWN:
		return 0;
	case EZWM_COVERCHILD:
	{
		if (Transparent)
		{
			BLENDFUNCTION bf = { 0,0, Transparent,0 };
			AlphaBlend(wParam, 0, 0, ezWnd->Width, ezWnd->Height, hdcBlack, 0, 0, ezWnd->Width, ezWnd->Height, bf);
		}

	}
		return 0;
	case EZWM_USER_NOTIFY:
		switch (wParam)
		{
		case 0:
			//����͸����
			Transparent = lParam;
		}
		return 0;
	case EZWM_DESTROY:
		DeleteMemDC(hdcBlack);
		return 0;
	}
	return 0;
}