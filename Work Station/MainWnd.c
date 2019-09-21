#include<Windows.h>
#include"EasyWindow.h"
#include"Global.h"
#include<math.h>
#include"GaussianBlur.h"

EZWND CreateMainParentWindow(int x, int y, int Width, int Height, int iCmdShow, EZWNDPROC ezWndProc)
{
	EZWND ezParent, ezWnd;
	DWORD WinStyle = WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_POPUP | WS_THICKFRAME;
	ezParent = CreateEZParentWindowEx(EZS_PARENT | EZS_OVERLAPPEDWINDOW, x, y, Width, Height,
		WinStyle, MainParentProc, 0, 0);
	ezParent->Extend->hExtend[0] = ezWnd = CreateEZWindow(ezParent, 0, APPBAR_HEIGHT, Width, Height - APPBAR_HEIGHT, ezWndProc);

	((pEZSE)ezParent->Extend)->hFont = NULL;
	ezParent->Extend->Title = 0;
	EZSendMessage(ezParent, EZWM_SETTEXT, szAppName, lstrlen(szAppName));


	((pEZSE)ezParent->Extend)->BackGroundColor = RGB(255, 255, 255);

	((pEZSE)ezParent->Extend)->ForeGroundColor = RGB(0, 0, 0);

	((pEZSE)ezParent->Extend)->MouseHold = FALSE;


	((pEZSE)ezParent->Extend)->IsFontUserControl = -1;


	//最后再设置一遍，防止之前被防野数据机制清零。
	ezParent->IsStyleWindow = TRUE;
	ezParent->EZStyle = EZS_PARENT | EZS_OVERLAPPEDWINDOW;

	EZRepaint(ezParent, 0);
	ShowWindow(ezParent->hParent, iCmdShow);
	UpdateWindow(ezParent->hParent);
	return ezWnd;
}

EZWNDPROC MainParentProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case EZWM_CREATE:
		if ((ezWnd->EZStyle & MKDW(00000000, 00000000, 00000000, 11111111)) == EZS_OVERLAPPEDWINDOW)
		{
			//这里！！创建关闭，缩小，放大按钮！！！
			int BtnLen = floor(EZWND_CAP_HEIGHT * 1.618);
			ezWnd->Extend->hExtend[1] = CreateEZWindow(ezWnd, ezWnd->Width - BtnLen, 0, BtnLen, EZWND_CAP_HEIGHT, EZStyle_WndCloseProc);
			ezWnd->Extend->hExtend[2] = CreateEZWindow(ezWnd, ezWnd->Width - (BtnLen << 1), 0, BtnLen, EZWND_CAP_HEIGHT, EZStyle_WndMaxProc);
			ezWnd->Extend->hExtend[3] = CreateEZWindow(ezWnd, ezWnd->Width - BtnLen * 3, 0, BtnLen, EZWND_CAP_HEIGHT, EZStyle_WndMinProc);
			//ezWnd->DrawOnNC = 1;
			//MARGINS margins = { 0,10,0,0 };
			//DwmExtendFrameIntoClientArea(ezWnd->hParent, &margins);
			////Sleep(0);
		}
		return 0;

	case EZWM_SETFONT:

		if (lParam)
		{
			int OldHeight = ((LOGFONT*)lParam)->lfHeight;
			((LOGFONT*)lParam)->lfHeight = EZWND_CAP_HEIGHT * 0.5;
			EZStyle_DefaultProc(ezWnd, message, wParam, lParam);
			((LOGFONT*)lParam)->lfHeight = OldHeight;
		}
		else
		{
			EZStyle_DefaultProc(ezWnd, message, wParam, lParam);
		}
		return 0;
	case EZWM_DRAW:

	{
		TCHAR* pTitle;
		pTitle = ((pEZSE)ezWnd->Extend)->Title;
		if (((pEZSE)ezWnd->Extend)->hFont)
		{
			SelectObject(wParam, ((pEZSE)ezWnd->Extend)->hFont);
			TextOut(wParam, 0, 0, pTitle, lstrlen(pTitle));
		}
		else
		{
			HFONT OldFont, hFont;
			hFont = CreateFont(APPBAR_HEIGHT * 0.5, 0, 0, 0, 550, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("微软雅黑"));
			OldFont = SelectObject(wParam, hFont);
			TextOut(wParam, APPBAR_HEIGHT >> 1, APPBAR_HEIGHT * 0.25, pTitle, lstrlen(pTitle));
			DeleteObject(SelectObject(wParam, OldFont));
		}

		return 0;
	}


	////case EZWM_WINNCDRAW:
	case EZWM_WINNCACTIVATE:
	{
		HDC hdc = GetDC(ezWnd->hParent);
		BitBlt(hdc, 0, 0, ezWnd->Width, ezWnd->Height, ezWnd->TopWndExtend->hdcTop, 0, 0, SRCCOPY);
		ReleaseDC(ezWnd->hParent, hdc);
		////这是我能想出来的最快的操作了
		return 1;
	}


	case EZWM_WINNCHITTEST:
	{
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);
		ScreenToClient(ezWnd->hParent, &pt);
		if (pt.y <= APPBAR_HEIGHT)
		{
			RECT rect;
			SetRect(&rect, ezWnd->Width - 3 * floor(EZWND_CAP_HEIGHT * 1.618), 0, ezWnd->Width, EZWND_CAP_HEIGHT);

			if (PtInRect(&rect, pt))
			{
				return EZHTCLIENT;
			}
			else
			{
				return EZHTCAPTION;
			}
		}
		else
		{
			return EZHTCLIENT;
		}



		//return 0;
	}

	case EZWM_SIZE:

		if (!IsIconic(ezWnd->hParent))
		{
			if (ezWnd->Extend->hExtend[0])
			{
				MoveEZWindow(ezWnd->Extend->hExtend[0], 0, APPBAR_HEIGHT, ezWnd->Width, ezWnd->Height - APPBAR_HEIGHT, 0);
			}

			if ((ezWnd->EZStyle & MKDW(00000000, 00000000, 00000000, 11111111)) == EZS_OVERLAPPEDWINDOW)
			{
				int BtnLen = floor(EZWND_CAP_HEIGHT * 1.618);

				MoveEZWindow(ezWnd->Extend->hExtend[1], ezWnd->Width - BtnLen, 0, BtnLen, EZWND_CAP_HEIGHT, 0);
				MoveEZWindow(ezWnd->Extend->hExtend[2], ezWnd->Width - (BtnLen << 1), 0, BtnLen, EZWND_CAP_HEIGHT, 0);
				MoveEZWindow(ezWnd->Extend->hExtend[3], ezWnd->Width - BtnLen * 3, 0, BtnLen, EZWND_CAP_HEIGHT, 0);

			}
		}
		return 0;
	case EZWM_GETMINMAXINFO:
	{
		PMINMAXINFO pMMInfo;
		RECT rect;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

		pMMInfo = lParam;


		//AdjustWindowRect(&rect, GetWindowLong(ezWnd->hParent, GWL_STYLE), 0);

		pMMInfo->ptMaxPosition.x = rect.left;// -(xborder << 1);
		pMMInfo->ptMaxPosition.y = rect.top;// -(yborder << 1);
		pMMInfo->ptMaxSize.x = rect.right - rect.left;// +(xborder << 2);
		pMMInfo->ptMaxSize.y = rect.bottom - rect.top;// +(yborder << 2);

		pMMInfo->ptMinTrackSize.x = MIN_UI_WIDTH;
		pMMInfo->ptMinTrackSize.y = MIN_UI_HEIGHT;
		return 1;
	}




	case EZWM_WINNCCALCSIZE:
	{
		if (wParam)
		{

			/*if (IsZoomed(ezWnd->hParent))
			{
				return 0;
			}
			else*/
			{
				NCCALCSIZE_PARAMS* NCCSParam;

				NCCSParam = (NCCALCSIZE_PARAMS*)lParam;
				//RECT rect = { 0};
				AdjustWindowRect(&(NCCSParam->rgrc[0]), GetWindowLong(ezWnd->hParent, GWL_STYLE), 0);
				return 0;
			}

		}
		//	InvalidateRect(hwnd, 0, 0);
		return 1;
	}

	case EZWM_CLOSE:
		return EZSendMessage(ezWnd->Extend->hExtend[0], EZWM_CLOSE, wParam, lParam);



	}
	return EZStyle_DefaultProc(ezWnd, message, wParam, lParam);
}


EZWNDPROC MainWndProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	static BOOL IsBorderOpened = 0;//这是目标状态
	static int BorderWidth = LBORDER_NARROW;//这是当前状态
	static int TimerID = -1;//计时器

	static HBITMAP BkgndPic = 0;
	static HDC BkgndDC = 0;
	static BITMAP BkgndBmp;

	static HDC BkBlurPic = 0;

	switch (message)
	{
	case EZWM_CREATE:
		//初始化GDI对象
		InitGDIObject();
		ScrollBar = CreateEZStyleWindow(ezWnd, TEXT(""), EZS_CHILD | EZS_CHILD_VSCROLL, 0, 0, 0, 0);
		PageHolder = CreateEZWindow(ezWnd, 0, 0, 0, 0, PageHolderProc);
		LBorder = CreateEZWindow(ezWnd, 0, 0, 0, 0, LeftBorderProc);
		
		//为了阴影也能覆盖滚动条，做这个修改
		ScrollBar->ezWndProc = NewScrollBarProc;
		//发一条假消息初始化
		NewScrollBarProc(ScrollBar, EZWM_CREATE, 0, 0);
		

		BkgndDC = CreateCompatibleDC(ezWnd->hdc);
		BkBlurPic = GetMemDC(ezWnd->hdc, 1, 1);

		EZSendMessage(ezWnd, EZWM_UPDATE_SETTINGS, 0, 0);
		return 0;

	case EZWM_UPDATE_SETTINGS:

		BkgndPic = LoadPicFromFile(BkgndPicPath);
		if (BkgndPic)
		{
			GetObject(BkgndPic, sizeof(BITMAP), &BkgndBmp);
			DeleteObject(SelectObject(BkgndDC, BkgndPic));

			AdjustMemDC(BkBlurPic, ezWnd->hdc, ezWnd->Width, ezWnd->Height);
			AdjustAndBlurBkPic(BkgndDC, BkgndBmp.bmWidth, BkgndBmp.bmHeight, BkBlurPic, ezWnd->Width, ezWnd->Height);
		}
		EZRepaint(ezWnd, 0);
		return 0;

	case EZWM_SIZE:
		MoveEZWindow(LBorder, 0, 0, BorderWidth, ezWnd->Height, 0);
		MoveEZWindow(PageHolder, LBORDER_NARROW, 0, ezWnd->Width - LBORDER_NARROW - 18, ezWnd->Height, 0);
		//这里！貌似所有的页面都需要使用滚动条
		//	干脆让滚动条和PageHolder并列，并且将PageHolder作为滚动层好了
		MoveEZWindow(ScrollBar, ezWnd->Width - 18, 0, 18, ezWnd->Height, 0);

		EZSendMessage(ScrollBar, EZWM_SETSCROLLPOS, 0, ezWnd->Height);
		
		//调整背景图
		if (BkgndPic)
		{
			AdjustMemDC(BkBlurPic, ezWnd->hdc, ezWnd->Width, ezWnd->Height);
			AdjustAndBlurBkPic(BkgndDC, BkgndBmp.bmWidth, BkgndBmp.bmHeight, BkBlurPic, ezWnd->Width, ezWnd->Height);
		}
		
		return 0;
	case EZWM_SCROLLPOSCHANGE:
		ScrollEZWindow(PageHolder, 0, -(int)wParam, 0);
		return 0;
	case EZWM_COMMAND:
	{
		if (lParam == LBorder)
		{
			switch (wParam)
			{
			case 0:
				IsBorderOpened = !IsBorderOpened;
				SetPageTransparent(IsBorderOpened);
				//EZSendMessage(ScrollBar, EZWM_USER_NOTIFY, 1, IsBorderOpened);
				ScrollBar->MouseMsgRecv = IsBorderOpened ? 2 : 1;
				if (TimerID == -1)
				{
					TimerID = SetEZTimer(ezWnd, ANIMATION_TIMESPACE);
				}
				break;
			default:
				if (IsBorderOpened)
				{
					IsBorderOpened = 0;
					SetPageTransparent(IsBorderOpened);
					//EZSendMessage(ScrollBar, EZWM_USER_NOTIFY, 1, IsBorderOpened);
					ScrollBar->MouseMsgRecv = IsBorderOpened ? 2 : 1;
					if (TimerID == -1)
					{
						TimerID = SetEZTimer(ezWnd, ANIMATION_TIMESPACE);
					}
				}
			}
		}
		
		
		return 0;
	}
	case EZWM_TIMER:
		if (IsBorderOpened)
		{
			BorderWidth += ceil(0.3 * (LBORDER_WIDE - BorderWidth));
			
			if (BorderWidth == LBORDER_WIDE)
			{
				KillEZTimer(ezWnd, TimerID);
				TimerID = -1;
			}
		}
		else
		{
			BorderWidth -= ceil(0.3 * (BorderWidth - LBORDER_NARROW));
			if (BorderWidth == LBORDER_NARROW)
			{
				KillEZTimer(ezWnd, TimerID);
				TimerID = -1;
			}
		}
		MoveEZWindow(LBorder, 0, 0, BorderWidth, ezWnd->Height, 0);
		EZSendMessage(PageHolder, EZWM_USER_NOTIFY, 0, 140 * (BorderWidth - LBORDER_NARROW) / (LBORDER_WIDE - LBORDER_NARROW));

		EZSendMessage(ScrollBar, EZWM_USER_NOTIFY, 0, 140 * (BorderWidth - LBORDER_NARROW) / (LBORDER_WIDE - LBORDER_NARROW));
		EZRepaint(ezWnd, 0);
		return 0;
	case EZWM_LBUTTONDOWN:
		if (IsBorderOpened)
		{
			IsBorderOpened = 0;
			SetPageTransparent(IsBorderOpened);
			//EZSendMessage(ScrollBar, EZWM_USER_NOTIFY, 1, IsBorderOpened);
			ScrollBar->MouseMsgRecv = IsBorderOpened ? 2 : 1;
			if (TimerID == -1)
			{
				TimerID = SetEZTimer(ezWnd, ANIMATION_TIMESPACE);
			}
		}
		return 0;
	case EZWM_DRAW:
	{
		if (BkgndPic)
		{
			BitBlt(wParam, 0, 0, ezWnd->Width, ezWnd->Height, BkBlurPic, 0, 0, SRCCOPY);
		}
		else
		{
			HBRUSH OldBrush = SelectObject(wParam, CreateSolidBrush(BackgroundColor));
			PatBlt(wParam, 0, 0, ezWnd->Width, ezWnd->Height, PATCOPY);
			DeleteObject(SelectObject(wParam, OldBrush));
		}
		//SelectObject(wParam, BkgndBitmap);
	}
		
		break;
	case EZWM_DESTROY:

		DeleteDC(BkgndDC);

		if (BkgndPic)
			DeleteObject(BkgndPic);


		DeleteMemDC(BkBlurPic);


		PostQuitMessage(0);
		break;

	}
	return 0;
}


BOOL AdjustAndBlurBkPic(HDC hdcBk,int x1,int y1,HDC hdcBlur,int x2,int y2)
{
	SetStretchBltMode(hdcBlur, HALFTONE);
	if (x1 * y2  < x2 * y1)	//if (x1 / y1 < x2 / y2)
	{
		//目标更长
		StretchBlt(hdcBlur, 0, 0, x2, y2, hdcBk, 0,(y1 - (y2*x1)/x2)/2 , x1, (y2 * x1) / x2, SRCCOPY);
	}
	else
	{
		StretchBlt(hdcBlur, 0, 0, x2, y2, hdcBk, (x1 - (x2 * y1) / y2) / 2,0, (x2 * y1)/ y2, y1, SRCCOPY);
		//StretchBlt(hdcBlur, 0, 0, x2, y2, hdcBk, 0, (y1 - (y2 * x1) / x2) / 2, x1, (y2 * x1) / x2, SRCCOPY);
	}
	//拉出来模糊处理
	BITMAPINFOHEADER   bi;

	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = x2;
	bi.biHeight = y2;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	HBITMAP hOrgdBitmap = SelectObject(hdcBlur, CreateCompatibleBitmap(hdcBlur, 1, 1));
	PBYTE pByte = malloc(x2 * y2 * 4);
	GetDIBits(hdcBlur, hOrgdBitmap, 0, y2 ,pByte, &bi, DIB_RGB_COLORS);
	
	GaussianBlurFilter(pByte, pByte, x2, y2, x2 * 4, 10);

	SetDIBits(hdcBlur, hOrgdBitmap, 0, y2, pByte, &bi, DIB_RGB_COLORS);
	
	
	free(pByte);
	DeleteObject(SelectObject(hdcBlur, hOrgdBitmap));


	return 0;
}


EZWNDPROC NewScrollBarProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	static int Transparent;
	static HDC hdcBlack;
	static BOOL BlockInput = 0;

	switch (message)
	{
	case EZWM_CREATE:
		//显然是个假消息...
		hdcBlack = GetMemDC(ezWnd->hdc, 1, 1);
		Transparent = 0;
		ezWnd->Transparent = 160;
		return 0;

	case EZWM_SIZE:
		AdjustMemDC(hdcBlack, ezWnd->hdc, ezWnd->Width, ezWnd->Height);
		PatBlt(hdcBlack, 0, 0, ezWnd->Width, ezWnd->Height, BLACKNESS);

		break;
	case EZWM_COVERCHILD:
	{
		if (Transparent)
		{
			BLENDFUNCTION bf = { 0,0, Transparent,0 };
			AlphaBlend(wParam, 0, 0, ezWnd->Width, ezWnd->Height, hdcBlack, 0, 0, ezWnd->Width, ezWnd->Height, bf);
		}

	}
	break;
	case EZWM_TRANSDRAW:
		PatBlt(wParam, 0, 0, ezWnd->Width, ezWnd->Height, WHITENESS);
		break;
	case EZWM_USER_NOTIFY:
		switch (wParam)
		{
		case 0:
			//设置透明度
			Transparent = lParam;
			break;
		}
		return 0;
	break;
	case EZWM_DESTROY:
		DeleteMemDC(hdcBlack);
		break;
	}

	return EZStyle_ScrollChildProc(ezWnd, message, wParam, lParam);
}
