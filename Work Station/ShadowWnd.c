#include<Windows.h>
#include"Global.h"
#include"GaussianBlur.h"

//这里是有关于Windows里隐藏的WM_SYSCOMMAND的消息定义
#define SC_SIZELEFT (SC_SIZE + 1)
#define SC_SIZERIGHT (SC_SIZE + 2)
#define SC_SIZETOP (SC_SIZE + 3)
#define SC_SIZELEFTTOP (SC_SIZE + 4)
#define SC_SIZERIGHTTOP (SC_SIZE + 5)
#define SC_SIZEBOTTOM (SC_SIZE + 6)
#define SC_SIZELEFTBOTTOM (SC_SIZE + 7)
#define SC_SIZERIGHTBOTTOM (SC_SIZE + 8)


HDC hdcCompose[8] = { 0 };
PBYTE CornerShadowData;
BYTE BorderShadowData[SHADOW_MARIGN * 2 * 4] = { 0 };
int OldWindowCX = 0, OldWindowCY = 0;//用于存储窗口大小以判断是否发生过变化

LRESULT CALLBACK ShadowWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static int Width, Height;
	switch (message)
	{
	case WM_CREATE:
	{
		/*int test = SetWindowLongPtr(hwnd, 0, ((LPCREATESTRUCT)lParam)->lpCreateParams);
		Sleep(0);*/
	}
	SetWindowLongPtr(hwnd, 0, ((LPCREATESTRUCT)lParam)->lpCreateParams);
	return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;

	case WM_NCHITTEST:
	{
		//这里得到的是相对全屏的鼠标坐标，所以只要先确定主窗口的位置就行，这里完全可以不用进行8个阴影窗口的区分。
		RECT ParentRect;
		GetWindowRect(MainWnd->hParent, &ParentRect);
		int CursorX, CursorY;
		int HittestArr[5][5] = {
		{HTTRANSPARENT	,HTTRANSPARENT	,HTTRANSPARENT	,HTTRANSPARENT		,HTTRANSPARENT },
		{HTTRANSPARENT	,HTCLIENT		,HTCLIENT		,HTCLIENT			,HTTRANSPARENT },
		{HTTRANSPARENT	,HTCLIENT		,HTERROR		,HTCLIENT			,HTTRANSPARENT },
		{HTTRANSPARENT	,HTCLIENT		,HTCLIENT		,HTCLIENT			,HTTRANSPARENT },
		{HTTRANSPARENT	,HTTRANSPARENT	,HTTRANSPARENT	,HTTRANSPARENT		,HTTRANSPARENT }
		};
		
		if (GET_X_LPARAM(lParam) < ParentRect.left - SHADOW_HITTEST)
		{
			CursorX = 0;
		}
		else if (GET_X_LPARAM(lParam) < ParentRect.left)
		{
			CursorX = 1;
		}
		else if (GET_X_LPARAM(lParam) < ParentRect.right - SHADOW_HITTEST)
		{
			CursorX = 2;
		}
		else if (GET_X_LPARAM(lParam) < ParentRect.right + SHADOW_HITTEST)
		{
			CursorX = 3;
		}
		else
		{
			CursorX = 4;
		}

		if (GET_Y_LPARAM(lParam) < ParentRect.top - SHADOW_HITTEST)
		{
			CursorY = 0;
		}
		else if (GET_Y_LPARAM(lParam) < ParentRect.top)
		{
			CursorY = 1;
		}
		else if (GET_Y_LPARAM(lParam) < ParentRect.bottom - SHADOW_HITTEST)
		{
			CursorY = 2;
		}
		else if (GET_Y_LPARAM(lParam) < ParentRect.bottom + SHADOW_HITTEST)
		{
			CursorY = 3;
		}
		else
		{
			CursorY = 4;
		}

		return HittestArr[CursorY][CursorX];

	}
	break;
	case WM_SETCURSOR:
	{
		HINSTANCE hInst = GetModuleHandle(0);
		int CursorList[8] = { IDC_SIZENWSE ,IDC_SIZENS,IDC_SIZENESW ,IDC_SIZEWE,IDC_SIZEWE,IDC_SIZENESW ,IDC_SIZENS,IDC_SIZENWSE };
		HCURSOR hcs = LoadCursor(NULL, CursorList[GetWindowLongPtr(hwnd, 0)]);
		SetCursor(hcs);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		int SysCommandList[8] = { SC_SIZELEFTTOP, SC_SIZETOP, SC_SIZERIGHTTOP, SC_SIZELEFT, SC_SIZERIGHT, SC_SIZELEFTBOTTOM, SC_SIZEBOTTOM, SC_SIZERIGHTBOTTOM };
		SendMessage(MainWnd->hParent, WM_SYSCOMMAND, SysCommandList[GetWindowLongPtr(hwnd, 0)], 0);
	}

	return 0;

	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

//这个函数（目前）也放在这里的原因是，装这个钩子的目的就是为了同步阴影窗口
LRESULT CALLBACK MainWndHookProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		//其实这是个假消息...

		HDC hdc = GetDC(hwnd);
		for (int i = 0; i < 8; i++)
		{
			hdcCompose[i] = CreateCompatibleDC(hdc);
		}
		ReleaseDC(hwnd, hdc);

		//这里初始化角落阴影（左上角样板），以及边框阴影的一条线
		CornerShadowData = malloc(SHADOW_MARIGN * SHADOW_MARIGN * 9 * 4);
		memset(CornerShadowData, 0, SHADOW_MARIGN * SHADOW_MARIGN * 9 * 4);
		for (int cy = SHADOW_MARIGN; cy < 3 * SHADOW_MARIGN; cy++)
		{
			for (int cx = SHADOW_MARIGN; cx < 3 * SHADOW_MARIGN; cx++)
			{
				(CornerShadowData + 4 * (cy * (3 * SHADOW_MARIGN) + cx))[3] = 100;
			}
		}
		//高斯模糊
		AlphaOnlyGaussianBlurFilter(CornerShadowData, CornerShadowData, 3 * SHADOW_MARIGN, 3 * SHADOW_MARIGN, 3 * SHADOW_MARIGN * 4, 4);


		for (int cx = SHADOW_MARIGN; cx < 2 * SHADOW_MARIGN; cx++)
		{
			(BorderShadowData + 4 * cx)[3] = 100;
		}
		//高斯模糊
		AlphaOnlyGaussianBlurFilter(BorderShadowData, BorderShadowData, SHADOW_MARIGN * 2, 1, SHADOW_MARIGN * 2 * 4, 4);

		if (lParam == 0)
		{
			return 0;
		}
	}
	break;
	case WM_WINDOWPOSCHANGED:
	{
		
	//	CallWindowProc(OldMainWndProc, hwnd, message, wParam, lParam);
		if (IsIconic(hwnd) || IsZoomed(hwnd))
		{
			OldWindowCX = 0;
			OldWindowCY = 0;
			for (int i = 0; i < 8; i++)
				ShowWindow(ShadowWnd[i], SW_HIDE);
		}
		else
		{
			PWINDOWPOS WndPos = lParam;

			int ArrX[8] = { WndPos->x - SHADOW_MARIGN,WndPos->x,WndPos->x + WndPos->cx,WndPos->x - SHADOW_MARIGN,WndPos->x + WndPos->cx,WndPos->x - SHADOW_MARIGN,WndPos->x,WndPos->x + WndPos->cx };
			int ArrY[8] = { WndPos->y - SHADOW_MARIGN,WndPos->y - SHADOW_MARIGN,WndPos->y - SHADOW_MARIGN,WndPos->y,WndPos->y,WndPos->y + WndPos->cy,WndPos->y + WndPos->cy,WndPos->y + WndPos->cy };
			int ArrCX[8] = { SHADOW_MARIGN, WndPos->cx,SHADOW_MARIGN,SHADOW_MARIGN,SHADOW_MARIGN,SHADOW_MARIGN, WndPos->cx,SHADOW_MARIGN };
			int ArrCY[8] = { SHADOW_MARIGN,SHADOW_MARIGN,SHADOW_MARIGN, WndPos->cy, WndPos->cy,SHADOW_MARIGN,SHADOW_MARIGN,SHADOW_MARIGN };

			if (OldWindowCX == WndPos->cx && OldWindowCY == WndPos->cy)
			{
				//大小保持不变
				for (int i = 0; i < 8; i++)
				{

					//MoveWindow(ShadowWnd[i], ArrX[i], ArrY[i], ArrCX[i], ArrCY[i], 0);
					SetWindowPos(ShadowWnd[i], hwnd,
						ArrX[i], ArrY[i], ArrCX[i], ArrCY[i],
						SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOREDRAW | SWP_NOREPOSITION | SWP_NOSENDCHANGING);

				}
			}
			else
			{
				OldWindowCX = WndPos->cx;
				OldWindowCY = WndPos->cy;
			}
			

		}

		break;
	}
	case WM_SIZE:
	{
		if (IsIconic(hwnd) || IsZoomed(hwnd))
		{
			break;
		}
		//生成阴影图
		HDC hdc = GetDC(hwnd);

		RECT ParentRect;
		GetWindowRect(MainWnd->hParent, &ParentRect);
		int ArrX[8] = { ParentRect.left - SHADOW_MARIGN,ParentRect.left,ParentRect.right,ParentRect.left - SHADOW_MARIGN,ParentRect.right,ParentRect.left - SHADOW_MARIGN,ParentRect.left,ParentRect.right };
		int ArrY[8] = { ParentRect.top - SHADOW_MARIGN,ParentRect.top - SHADOW_MARIGN,ParentRect.top - SHADOW_MARIGN,ParentRect.top,ParentRect.top,ParentRect.bottom,ParentRect.bottom,ParentRect.bottom };
		
		int ArrCX[8] = { SHADOW_MARIGN, GET_X_LPARAM(lParam),SHADOW_MARIGN,SHADOW_MARIGN,SHADOW_MARIGN,SHADOW_MARIGN, GET_X_LPARAM(lParam),SHADOW_MARIGN };
		int ArrCY[8] = { SHADOW_MARIGN,SHADOW_MARIGN,SHADOW_MARIGN, GET_Y_LPARAM(lParam),GET_Y_LPARAM(lParam),SHADOW_MARIGN,SHADOW_MARIGN, SHADOW_MARIGN };
		for (int i = 0; i < 8; i++)
		{
			PBYTE Data = malloc(ArrCX[i] * ArrCY[i] * 4);
			memset(Data, 0, ArrCX[i] * ArrCY[i] * 4);
			HBITMAP hBmp = CreateCompatibleBitmap(hdc, ArrCX[i], ArrCY[i]);
			switch (i)
			{
			case 0:
			case 2:
			case 5:
			case 7:
			{
				int XReverse[8] = { 0,0,1,0,0,0,0,1 };
				int YReverse[8] = { 0,0,0,0,0,1,0,1 };
				for (int CopyY = 0; CopyY < SHADOW_MARIGN; CopyY++)
					for (int CopyX = 0; CopyX < SHADOW_MARIGN; CopyX++)
					{
						(Data + 4 * ((YReverse[i] ? ArrCY[i] - CopyY - 1 : CopyY) * ArrCX[i] + (XReverse[i] ? ArrCX[i] - CopyX - 1 : CopyX)))[3] = (CornerShadowData + 4 * ((CopyY * SHADOW_MARIGN * 3) + CopyX))[3];
					}


				break;
			}


			case 1:
			{
				for (int CopyY = 0; CopyY < SHADOW_MARIGN; CopyY++)
					for (int CopyX = 0; CopyX < SHADOW_MARIGN; CopyX++)
					{
						(Data + 4 * ((CopyY * ArrCX[i]) + CopyX))[3] = (CornerShadowData + 4 * ((CopyY * SHADOW_MARIGN * 3) + CopyX + SHADOW_MARIGN))[3];
					}

				for (int CopyY = 0; CopyY < SHADOW_MARIGN; CopyY++)
					for (int CopyX = 0; CopyX < SHADOW_MARIGN; CopyX++)
					{
						(Data + 4 * ((CopyY * ArrCX[i]) + ArrCX[i] - CopyX - 1))[3] = (CornerShadowData + 4 * ((CopyY * SHADOW_MARIGN * 3) + CopyX + SHADOW_MARIGN))[3];
					}

				for (int CopyY = 0; CopyY < SHADOW_MARIGN; CopyY++)
					for (int CopyX = SHADOW_MARIGN; CopyX < ArrCX[i] - SHADOW_MARIGN; CopyX++)
					{
						(Data + 4 * (CopyY * ArrCX[i] + CopyX))[3] = (BorderShadowData + 4 * (CopyY))[3];
					}

				break;
			}
			case 3:
			{
				for (int CopyY = 0; CopyY < SHADOW_MARIGN; CopyY++)
					for (int CopyX = 0; CopyX < SHADOW_MARIGN; CopyX++)
					{
						(Data + 4 * ((CopyY * ArrCX[i]) + CopyX))[3] = (CornerShadowData + 4 * (((CopyY + SHADOW_MARIGN) * SHADOW_MARIGN * 3) + CopyX))[3];
					}

				for (int CopyY = 0; CopyY < SHADOW_MARIGN; CopyY++)
					for (int CopyX = 0; CopyX < SHADOW_MARIGN; CopyX++)
					{
						(Data + 4 * (((ArrCY[i] - CopyY - 1) * ArrCX[i]) + CopyX))[3] = (CornerShadowData + 4 * (((CopyY + SHADOW_MARIGN) * SHADOW_MARIGN * 3) + CopyX))[3];
					}

				for (int CopyY = SHADOW_MARIGN; CopyY < ArrCY[i] - SHADOW_MARIGN; CopyY++)
					for (int CopyX = 0; CopyX < SHADOW_MARIGN; CopyX++)
					{
						(Data + 4 * (CopyY * ArrCX[i] + CopyX))[3] = (BorderShadowData + 4 * (CopyX))[3];
					}
				break;
			}
			case 4:
			{
				for (int CopyY = 0; CopyY < SHADOW_MARIGN; CopyY++)
					for (int CopyX = 0; CopyX < SHADOW_MARIGN; CopyX++)
					{
						(Data + 4 * ((CopyY * ArrCX[i]) + ArrCX[i] - CopyX - 1))[3] = (CornerShadowData + 4 * (((CopyY + SHADOW_MARIGN) * SHADOW_MARIGN * 3) + CopyX))[3];
					}

				for (int CopyY = 0; CopyY < SHADOW_MARIGN; CopyY++)
					for (int CopyX = 0; CopyX < SHADOW_MARIGN; CopyX++)
					{
						(Data + 4 * (((ArrCY[i] - CopyY - 1) * ArrCX[i]) + ArrCX[i] - CopyX - 1))[3] = (CornerShadowData + 4 * (((CopyY + SHADOW_MARIGN) * SHADOW_MARIGN * 3) + CopyX))[3];
					}

				for (int CopyY = SHADOW_MARIGN; CopyY < ArrCY[i] - SHADOW_MARIGN; CopyY++)
					for (int CopyX = 0; CopyX < SHADOW_MARIGN; CopyX++)
					{
						(Data + 4 * (CopyY * ArrCX[i] + CopyX))[3] = (BorderShadowData + 4 * (SHADOW_MARIGN - CopyX - 1))[3];
					}

				break;
			}
			case 6:
			{
				for (int CopyY = 0; CopyY < SHADOW_MARIGN; CopyY++)
					for (int CopyX = 0; CopyX < SHADOW_MARIGN; CopyX++)
					{
						(Data + 4 * (((ArrCY[i] - CopyY - 1) * ArrCX[i]) + CopyX))[3] = (CornerShadowData + 4 * ((CopyY * SHADOW_MARIGN * 3) + CopyX + SHADOW_MARIGN))[3];
					}

				for (int CopyY = 0; CopyY < SHADOW_MARIGN; CopyY++)
					for (int CopyX = 0; CopyX < SHADOW_MARIGN; CopyX++)
					{
						(Data + 4 * (((ArrCY[i] - CopyY - 1) * ArrCX[i]) + ArrCX[i] - CopyX - 1))[3] = (CornerShadowData + 4 * ((CopyY * SHADOW_MARIGN * 3) + CopyX + SHADOW_MARIGN))[3];
					}

				for (int CopyY = 0; CopyY < SHADOW_MARIGN; CopyY++)
					for (int CopyX = SHADOW_MARIGN; CopyX < ArrCX[i] - SHADOW_MARIGN; CopyX++)
					{
						(Data + 4 * (CopyY * ArrCX[i] + CopyX))[3] = (BorderShadowData + 4 * (SHADOW_MARIGN - CopyY - 1))[3];
					}
				break;
			}
			}

			SetBitmapBits(hBmp, ArrCX[i] * ArrCY[i] * 4, Data);
			DeleteObject(SelectObject(hdcCompose[i], hBmp));

			free(Data);
			//位块传输
		}

		ReleaseDC(hwnd, hdc);


		SIZE sz = { 0 };
		POINT pt = { 0 };
		POINT ptscrn = { 0 };
		BLENDFUNCTION bf = { 0 };
		bf.AlphaFormat = AC_SRC_ALPHA;
		bf.SourceConstantAlpha = 255;
		
		for (int i = 0; i < 8; i++)
		{
			sz.cx = ArrCX[i];
			sz.cy = ArrCY[i];
			pt.x = 0;
			pt.y = 0;
			ptscrn.x = ArrX[i];
			ptscrn.y = ArrY[i];
			HDC hdcShadowWnd = GetDC(ShadowWnd[i]);
			UpdateLayeredWindow(ShadowWnd[i], hdcShadowWnd, &ptscrn, &sz, hdcCompose[i], &pt, 0, &bf, ULW_ALPHA);
			SetWindowPos(ShadowWnd[i], hwnd,
				0, 0, 0, 0,
				SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE | SWP_NOREDRAW | SWP_NOREPOSITION | SWP_NOSENDCHANGING);
			ReleaseDC(ShadowWnd[i], hdcShadowWnd);
		}

		break;
	}
	
	case WM_DESTROY:
	{
		HDC hdc = GetDC(hwnd);
		HBITMAP hSel = CreateCompatibleBitmap(hdc, 1, 1);
		ReleaseDC(hwnd, hdc);

		for (int i = 0; i < 8; i++)
		{
			DeleteObject(SelectObject(hdcCompose[i], hSel));
			DeleteDC(hdcCompose[i]);
			hdcCompose[i] = 0;
		}

		if (CornerShadowData)
		{
			free(CornerShadowData);
		}

		DeleteObject(hSel);
	}
	for (int i = 0; i < 8; i++)
		DestroyWindow(ShadowWnd[i]);
	break;


	case WM_STDIO_REDIRECT:
	{
		pVTEXT buf = InitVText();
	//	MessageBoxW(0, wParam, L"qwq", 0);
		SetVText(buf, wParam, lParam);
		free(wParam);
		CatVText(ConsoleText, buf);
		FreeVText(buf);

		if(MainWnd)EZRepaint(MainWnd, 0);
		break;
	}
		
	}
	return CallWindowProc(OldMainWndProc, hwnd, message, wParam, lParam);
}