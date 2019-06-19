#include<Windows.h>
#include"EasyWindow.h"
#include"Global.h"


BOOL DrawMenuBtn(HDC hdc, int x, int y, int Width, int Height, int bStretch, COLORREF Color);
BOOL DrawHomeBtn(HDC hdc, int x, int y, int Width, int Height, int bStretch, COLORREF Color);
BOOL DrawConsoleBtn(HDC hdc, int x, int y, int Width, int Height, int bStretch, COLORREF Color);

#define MAX_BUTTON_TEXT 32

typedef struct __tagFixedBtn
{
	EZWND Btn;
	TCHAR BtnText[MAX_BUTTON_TEXT];
	BOOL(*DrawProc)(HDC hdc, int x, int y, int Width, int Height,int bStretch,COLORREF Color);
	BOOL EnableStretch;
	EZWNDPROC PageProc;
	int PageID;
} FIXED_BTN,*pFIXED_BTN;


FIXED_BTN FixedBtn[] = { {0,TEXT(""),DrawMenuBtn,0,0,-1},{0,TEXT("主页"),DrawHomeBtn,1,MainPageProc,-1},{0,TEXT("控制台"),DrawConsoleBtn,1,ConsolePageProc,-1} };

#define FIXED_BTN_NUM (sizeof(FixedBtn) / sizeof(FixedBtn[0]))


typedef struct __tagBtnExtend
{
	int iHover;
	int TimerID;
	HDC hdcMem;
}BTN_EXTEND,*pBTN_EXTEND;

EZWNDPROC LeftBorderProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case EZWM_CREATE:
		//创建按钮
		for (int i = 0; i < FIXED_BTN_NUM; i++)
		{
			FixedBtn[i].Btn = CreateEZWindow(ezWnd, 0, 0, 0, 0, FixedBtnProc);
			FixedBtn[i].Btn->ezID = i;

			//创建相应的页（如果有）
			if (FixedBtn[i].PageProc)
			{
				FixedBtn[i].PageID = AddPage(FixedBtn[i].PageProc);
			}
		}

		

		SelectPage(0);


		return 0;

		
	case EZWM_SIZE:
		for (int i = 0; i < FIXED_BTN_NUM; i++)
		{
			MoveEZWindow(FixedBtn[i].Btn, 0, 30 + i * LBORDER_NARROW, ezWnd->Width, LBORDER_NARROW, 0);
		}
		return 0;
	case EZWM_DRAW:
	{
		HBRUSH OldBrush = SelectObject(wParam, CreateSolidBrush(LightBackgroundColor));
		PatBlt(wParam, 0, 0, ezWnd->Width, ezWnd->Height, PATCOPY);
		DeleteObject(SelectObject(wParam, OldBrush));
	}
		return 0;
	case EZWM_COMMAND:
	{
		//通知上级
		EZSendMessage(ezWnd->ezParent, EZWM_COMMAND, wParam, ezWnd);
		if (FixedBtn[wParam].PageID != -1)
		{
			SelectPage(FixedBtn[wParam].PageID);
		}
		
		return 0;
	}
	}
	return 0;
}

EZWNDPROC FixedBtnProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	pBTN_EXTEND Ext = ezWnd->Extend;
	switch (message)
	{
	case EZWM_CREATE:
		Ext = ezWnd->Extend = malloc(sizeof(BTN_EXTEND));
		Ext->iHover = 0;
		Ext->TimerID = -1;
		HDC hdc = GetDC(ezWnd->hParent);
		Ext->hdcMem = GetMemDC(hdc, LBORDER_NARROW * STRETCH, LBORDER_NARROW * STRETCH);
		ReleaseDC(ezWnd->hParent, hdc);

		ezWnd->Transparent = 0;

		
		return 0;
	
	case EZWM_MOUSECOME:
		if (Ext->TimerID == -1)
		{
			Ext->TimerID = SetEZTimer(ezWnd, ANIMATION_TIMESPACE);
		}
		return 0;
	case EZWM_MOUSELEAVE:
		if (Ext->TimerID == -1)
		{
			Ext->TimerID = SetEZTimer(ezWnd, ANIMATION_TIMESPACE);
		}
		return 0;
	case EZWM_TIMER:
		if (ezWnd->MouseOn)
		{
			
			if (Ext->iHover == 4)
			{
				KillEZTimer(ezWnd, Ext->TimerID);
				Ext->TimerID = -1;
			}
			else
			{
				Ext->iHover++;
				SetEZWndTransparent(ezWnd, 255 * Ext->iHover / 4.0);
			}
			
		}
		else
		{
			
			if (Ext->iHover == 0)
			{
				KillEZTimer(ezWnd, Ext->TimerID);
				Ext->TimerID = -1;
			}
			else
			{
				Ext->iHover--;
				SetEZWndTransparent(ezWnd, 255 * Ext->iHover / 4.0);
			}
			
		}
		EZRepaint(ezWnd, 0);
		return 0;
	case EZWM_DRAW:
	{
		SetStretchBltMode(wParam, HALFTONE);
		//SetStretchBltMode(Ext->hdcMem, HALFTONE);
		StretchBlt(Ext->hdcMem, 0, 0, ezWnd->Height * STRETCH, ezWnd->Height * STRETCH, wParam, 0, 0, ezWnd->Height, ezWnd->Height, SRCCOPY);
		SelectObject(Ext->hdcMem, GetStockObject(NULL_BRUSH));
		HPEN OldPen = SelectObject(Ext->hdcMem, CreatePen(PS_INSIDEFRAME, 2.5 * STRETCH, AppColor));
		Ellipse(Ext->hdcMem, ezWnd->Height * STRETCH / 6.0, ezWnd->Height * STRETCH / 6.0, ezWnd->Height * STRETCH * 5 / 6.0, ezWnd->Height * STRETCH * 5 / 6.0);
		
		DeleteObject(SelectObject(Ext->hdcMem, OldPen));
		StretchBlt(wParam, 0, 0, ezWnd->Height, ezWnd->Height, Ext->hdcMem, 0, 0, ezWnd->Height * STRETCH, ezWnd->Height * STRETCH, SRCCOPY);

		
		FixedBtn[ezWnd->ezID].DrawProc(wParam,
			ezWnd->Height / 4.0, ezWnd->Height / 4.0, ezWnd->Height / 2.0, ezWnd->Height / 2.0,
			FixedBtn[ezWnd->ezID].EnableStretch, RGB(255 * Ext->iHover / 4.0, 255 * Ext->iHover / 4.0, 255 * Ext->iHover / 4.0));

		HFONT OldFont = SelectObject(wParam, CreateAppFont(ezWnd->Height * (1.0/2.0), 400));
		SetBkMode(wParam, TRANSPARENT);
		TextOut(wParam, LBORDER_NARROW * (3 - 1.618), ezWnd->Height / 4.0, FixedBtn[ezWnd->ezID].BtnText, lstrlen(FixedBtn[ezWnd->ezID].BtnText));
		DeleteFont(SelectObject(wParam, OldFont));
	}
	return 0;
	case EZWM_TRANSDRAW:
	{


		
		SetStretchBltMode(wParam, HALFTONE);
		//SetStretchBltMode(Ext->hdcMem, HALFTONE);
		StretchBlt(Ext->hdcMem, 0, 0, ezWnd->Height * STRETCH, ezWnd->Height * STRETCH, wParam, 0, 0, ezWnd->Height, ezWnd->Height, SRCCOPY);
		SelectObject(Ext->hdcMem, GetStockObject(NULL_PEN));
		HBRUSH OldBrush = SelectObject(Ext->hdcMem, CreateSolidBrush(AppColor));
		Ellipse(Ext->hdcMem, ezWnd->Height * STRETCH / 6.0, ezWnd->Height * STRETCH / 6.0, ezWnd->Height * STRETCH * 5 / 6.0, ezWnd->Height * STRETCH * 5 / 6.0);

		DeleteObject(SelectObject(Ext->hdcMem, OldBrush));
		StretchBlt(wParam, 0, 0, ezWnd->Height, ezWnd->Height, Ext->hdcMem, 0, 0, ezWnd->Height * STRETCH, ezWnd->Height * STRETCH, SRCCOPY);

	



		/*HBRUSH OldBrush = SelectObject(wParam, CreateSolidBrush(AppColor));
		SelectObject(wParam, GetStockObject(NULL_PEN));
		Ellipse(wParam, ezWnd->Width / 6.0, ezWnd->Height / 6.0, ezWnd->Width * 5.0 / 6.0, ezWnd->Height * 5.0 / 6.0);
		DeleteObject(SelectObject(wParam, OldBrush));*/
	}
	return 0;

	case EZWM_LBUTTONDOWN:
	{
		EZSendMessage(ezWnd->ezParent, EZWM_COMMAND, ezWnd->ezID, ezWnd);
	}
	return 0;
	case EZWM_DESTROY:
		DeleteMemDC(Ext->hdcMem);
		free(ezWnd->Extend);
		return 0;
	}
	return 0;
}





BOOL DrawMenuBtn(HDC hdc, int x, int y, int Width, int Height, int bStretch, COLORREF Color)
{
	HDC hdcRender;
	if (bStretch)
	{
		hdcRender = GetMemDC(hdc, Width * STRETCH, Height * STRETCH);
		StretchBlt(hdcRender, 0, 0, Width * STRETCH, Height * STRETCH, hdc, x, y, Width, Height, SRCCOPY);
	}
	else
	{
		hdcRender = hdc;
	}

	HPEN OldPen = SelectObject(hdcRender, CreatePen(PS_SOLID, bStretch ? STRETCH : 1, Color));

	int IconWidth = Width * (bStretch ? STRETCH : 1);
	int IconHeight = Height * (bStretch ? STRETCH : 1);
	int IconX = (bStretch ? 0 : x);
	int IconY = (bStretch ? 0 : y);

	MoveToEx(hdcRender, IconX + IconWidth * 1.0 / 4.0, IconY + IconHeight * 3.0 / 8.0, 0);
	LineTo(hdcRender, IconX + IconWidth * 3.0 / 4.0, IconY + IconHeight * 3.0 / 8.0);

	MoveToEx(hdcRender, IconX + IconWidth * 1.0 / 4.0, IconY + IconHeight * 4.0 / 8.0, 0);
	LineTo(hdcRender, IconX + IconWidth * 3.0 / 4.0, IconY + IconHeight * 4.0 / 8.0);

	MoveToEx(hdcRender, IconX + IconWidth * 1.0 / 4.0, IconY + IconHeight * 5.0 / 8.0, 0);
	LineTo(hdcRender, IconX + IconWidth * 3.0 / 4.0, IconY + IconHeight * 5.0 / 8.0);

	DeleteObject(SelectObject(hdcRender, OldPen));
	if (bStretch)
	{
		SetStretchBltMode(hdc, HALFTONE);
		StretchBlt(hdc, x, y, Width, Height, hdcRender, 0, 0, Width * STRETCH, Height * STRETCH, SRCCOPY);
		DeleteMemDC(hdcRender);
	}

	return 0;
}


BOOL DrawHomeBtn(HDC hdc, int x, int y, int Width, int Height, int bStretch, COLORREF Color)
{
	HDC hdcRender;
	if (bStretch)
	{
		hdcRender = GetMemDC(hdc, Width * STRETCH, Height * STRETCH);
		StretchBlt(hdcRender, 0, 0, Width * STRETCH, Height * STRETCH, hdc, x, y, Width, Height, SRCCOPY);
	}
	else
	{
		hdcRender = hdc;
	}

	HPEN OldPen = SelectObject(hdcRender, CreatePen(PS_SOLID, bStretch ? STRETCH : 1, Color));

	int IconWidth = Width * (bStretch ? STRETCH : 1);
	int IconHeight = Height * (bStretch ? STRETCH : 1);
	int IconX = (bStretch ? 0 : x);
	int IconY = (bStretch ? 0 : y);

	//TODO: 这段代码太恶心了... 重构成Polygon吧

	MoveToEx(hdcRender, IconX + IconWidth / 2.0, IconY + IconHeight / 4.0, 0);
	LineTo(hdcRender, IconX + IconWidth / 4.0, IconY + IconHeight / 2.0);
	LineTo(hdcRender, IconX + IconWidth * 11 / 32.0, IconY + IconHeight / 2.0);
	LineTo(hdcRender, IconX + IconWidth * 11 / 32.0, IconY + IconHeight * 3 / 4.0);
	LineTo(hdcRender, IconX + IconWidth * 15 / 32.0, IconY + IconHeight * 3 / 4.0);
	LineTo(hdcRender, IconX + IconWidth * 15 / 32.0, IconY + IconHeight * 9 / 16.0);
	LineTo(hdcRender, IconX + IconWidth * 17 / 32.0, IconY + IconHeight * 9 / 16.0);
	LineTo(hdcRender, IconX + IconWidth * 17 / 32.0, IconY + IconHeight * 3 / 4.0);
	LineTo(hdcRender, IconX + IconWidth * 21 / 32.0, IconY + IconHeight * 3 / 4.0);
	LineTo(hdcRender, IconX + IconWidth * 21 / 32.0, IconY + IconHeight / 2.0);
	LineTo(hdcRender, IconX + IconWidth * 3 / 4.0, IconY + IconHeight / 2.0);
	LineTo(hdcRender, IconX + IconWidth / 2.0, IconY + IconHeight / 4.0);

	DeleteObject(SelectObject(hdcRender, OldPen));
	if (bStretch)
	{
		SetStretchBltMode(hdc, HALFTONE);
		StretchBlt(hdc, x, y, Width, Height, hdcRender, 0, 0, Width * STRETCH, Height * STRETCH, SRCCOPY);
		DeleteMemDC(hdcRender);
	}

	return 0;
}


BOOL DrawConsoleBtn(HDC hdc, int x, int y, int Width, int Height, int bStretch, COLORREF Color)
{
	HDC hdcRender;
	if (bStretch)
	{
		hdcRender = GetMemDC(hdc, Width * STRETCH, Height * STRETCH);
		StretchBlt(hdcRender, 0, 0, Width * STRETCH, Height * STRETCH, hdc, x, y, Width, Height, SRCCOPY);
	}
	else
	{
		hdcRender = hdc;
	}

	HPEN OldPen = SelectObject(hdcRender, CreatePen(PS_SOLID, bStretch ? STRETCH : 1, Color));

	int IconWidth = Width * (bStretch ? STRETCH : 1);
	int IconHeight = Height * (bStretch ? STRETCH : 1);
	int IconX = (bStretch ? 0 : x);
	int IconY = (bStretch ? 0 : y);

	//绘制一个 “>_”

	MoveToEx(hdcRender, IconX + IconWidth / 4.0, IconY + IconHeight * 3 / 8.0, 0);
	LineTo(hdcRender, IconX + IconWidth / 2.0, IconY + IconHeight / 2.0);
	LineTo(hdcRender, IconX + IconWidth / 4.0, IconY + IconHeight * 5 / 8.0);
	
	MoveToEx(hdcRender, IconX + IconWidth / 2.0, IconY + IconHeight * 11 / 16.0, 0);
	LineTo(hdcRender, IconX + IconWidth * 3 / 4.0, IconY + IconHeight * 11 / 16.0);

	DeleteObject(SelectObject(hdcRender, OldPen));
	if (bStretch)
	{
		SetStretchBltMode(hdc, HALFTONE);
		StretchBlt(hdc, x, y, Width, Height, hdcRender, 0, 0, Width * STRETCH, Height * STRETCH, SRCCOPY);
		DeleteMemDC(hdcRender);
	}

	return 0;
}

