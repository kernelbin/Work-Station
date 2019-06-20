#include<Windows.h>
#include"EasyWindow.h"
#include"Global.h"


typedef struct __tagVectorText
{
	int Capibility;
	int Length;
	TCHAR* Text;
}VECTORTEXT, VTEXT, * pVTEXT;


BOOL LocateCaretPos(HDC hdc, HFONT hFont, TCHAR Text[], TCHAR InputText[], int Width, int CaretPos, PPOINT pt);


pVTEXT InitVText();
BOOL FreeVText(pVTEXT pVText);
BOOL SetVText(pVTEXT pVText, TCHAR Str[], int len);



EZWNDPROC ConsolePageProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	static HFONT ConsoleFont;
	//这里无视其他地方的字体设置，使用固定字体
	static int CaretPos = 0;//第几个字符。


	static pVTEXT Text;
	static pVTEXT InputText;
	//static TCHAR Text[] = TEXT("Microsoft Windows [版本 10.0.18362.175]\r\n(c)2019 Microsoft Corporation。保留所有权利。\r\n\r\nC:\\Users\\11603>");//ezWnd->ezParent->ezParent->Extend->Title;

	//static TCHAR InputText[] = TEXT("sudo apt-get install minecraft");//ezWnd->ezParent->ezParent->Extend->Title;

	static int InputLen = 20;


	switch (message)
	{
	case EZWM_CREATE:
		ConsoleFont = CreateFont(16, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("新宋体"));

		Text = InitVText();
		InputText = InitVText();
		SetVText(Text, TEXT("Microsoft Windows [版本 10.0.18362.175]\r\n(c)2019 Microsoft Corporation。保留所有权利。\r\n\r\nC:\\Users\\11603>"), -1);
		SetVText(InputText, TEXT("sudo apt-get install minecraft"), -1);
		return 0;

	case EZWM_SIZE:

		return 0;

	case EZWM_DRAW:
	{
		PatBlt(wParam, 0, 0, ezWnd->Width, ezWnd->Height, BLACKNESS);
		SelectObject(wParam, ConsoleFont);
		SetBkMode(wParam, TRANSPARENT);
		SetTextColor(wParam, RGB(204, 204, 204));



		if (!Text->Text)
		{
			return 0;
		}
		int iMaxLen = lstrlen(Text->Text);
		int LastMove = 0;
		int xCount = 0;
		int yCount = 0;
		int iMove;

		TEXTMETRIC tm;
		GetTextMetrics(wParam, &tm);
		for (iMove = 0; iMove <= iMaxLen; iMove++)
		{
			if (Text->Text[iMove] == '\0')
			{
				//绘制当前行，并退出。
				SIZE sz;
				GetTextExtentPoint32(wParam, Text->Text + LastMove, iMove - LastMove, &sz);
				TextOut(wParam, 0, yCount, Text->Text + LastMove, iMove - LastMove);
				//yCount += tm.tmHeight;
				xCount = sz.cx;
				break;
			}
			else if (Text->Text[iMove] == '\r' && Text->Text[iMove + 1] == '\n')
			{
				//windows换行标记，绘制当前行，重新开始。
				TextOut(wParam, 0, yCount, Text->Text + LastMove, iMove - LastMove);
				yCount += tm.tmHeight;
				LastMove = iMove + 2;
				iMove++;
			}
			else if (Text->Text[iMove] == '\n')
			{
				//Linux换行标记，绘制当前行。
				TextOut(wParam, 0, yCount, Text->Text + LastMove, iMove - LastMove);
				yCount += tm.tmHeight;
				LastMove = iMove + 1;
			}
			else
			{
				//检查一下会不会长度超出窗口宽度
				SIZE sz, sz2;
				GetTextExtentPoint32(wParam, Text->Text + LastMove, iMove - LastMove, &sz);
				GetTextExtentPoint32(wParam, Text->Text + LastMove, iMove - LastMove + 1, &sz2);
				if (sz.cx <= ezWnd->Width && sz2.cx > ezWnd->Width)
				{
					//截断
					TextOut(wParam, 0, yCount, Text->Text + LastMove, iMove - LastMove);
					yCount += tm.tmHeight;
					LastMove = iMove;// +1;
				}
			}
		}


		SetTextColor(wParam, RGB(255, 255, 255));


		LastMove = 0;
		for (iMove = 0; iMove <= iMaxLen; iMove++)
		{
			if (InputText->Text[iMove] == '\0')
			{
				//绘制当前行，并退出。
				TextOut(wParam, xCount, yCount, InputText->Text + LastMove, iMove - LastMove);
				xCount = 0;
				yCount += tm.tmHeight;
				break;
			}
			else
			{
				//检查一下会不会长度超出窗口宽度
				SIZE sz, sz2;
				GetTextExtentPoint32(wParam, InputText->Text + LastMove, iMove - LastMove, &sz);
				GetTextExtentPoint32(wParam, InputText->Text + LastMove, iMove - LastMove + 1, &sz2);
				if (sz.cx + xCount <= ezWnd->Width && sz2.cx + xCount > ezWnd->Width)
				{
					//截断
					TextOut(wParam, xCount, yCount, InputText->Text + LastMove, iMove - LastMove);
					xCount = 0;
					yCount += tm.tmHeight;
					LastMove = iMove;// +1;
				}
			}
		}
	}
	return 0;

	case EZWM_CHAR:
		//字符消息

		switch (wParam)
		{
		case '\r':
			break;
		case '\n':
			break;
		case '\b':
			break;

		default:
			VTextInsertChar(InputText, wParam, CaretPos);
			CaretPos++;

			EZHideCaret(ezWnd);

			POINT pt;
			LocateCaretPos(ezWnd->hdc, ConsoleFont, Text->Text, InputText->Text, ezWnd->Width, CaretPos, &pt);
			EZSetCaretPos(ezWnd, pt.x, pt.y);
			EZRepaint(ezWnd, 0);

			EZShowCaret(ezWnd);

			break;
		}
		return 0;
		
	case EZWM_KEYDOWN:
		//处理上下左右键（左右切换输入，上下切换历史指令）
		if (wParam == 37)
		{
			if (CaretPos > 0)
			{
				CaretPos--;
			}
			else
			{
				MessageBeep(0);
			}

		}
		else if (wParam == 39)
		{
			if (CaretPos < InputText->Length)
			{
				CaretPos++;
			}
			else
			{
				MessageBeep(0);
			}

		}

		POINT pt;
		LocateCaretPos(ezWnd->hdc, ConsoleFont, Text->Text, InputText->Text, ezWnd->Width, CaretPos, &pt);
		EZSetCaretPos(ezWnd, pt.x, pt.y);

		return 0;

	case EZWM_SETFOCUS:

		EZCreateCaret(ezWnd, 0, 8, 4);

		//EZSetCaretPos(ezWnd, 0, 0);
		{

			POINT pt;
			LocateCaretPos(ezWnd->hdc, ConsoleFont, Text->Text, InputText->Text, ezWnd->Width, CaretPos, &pt);
			EZSetCaretPos(ezWnd, pt.x, pt.y);
		}
		EZShowCaret(ezWnd);
		return 0;

	case EZWM_KILLFOCUS:

		EZHideCaret(ezWnd);
		EZDestroyCaret();

		return 0;
	case EZWM_USER_NOTIFY:

		return 1200;

	case EZWM_DESTROY:
		DeleteObject(ConsoleFont);
		FreeVText(InputText);
		FreeVText(Text);
	}
	return 0;
}

BOOL LocateCaretPos(HDC hdc, HFONT hFont, TCHAR Text[], TCHAR InputText[], int Width, int CaretPos, PPOINT pt)
{
	//计算出位置并返回
	HFONT OldFont = SelectObject(hdc, hFont);


	int iMaxLen = lstrlen(Text);
	int LastMove = 0;
	int xCount = 0;
	int yCount = 0;
	int iMove;


	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);

	for (iMove = 0; iMove <= iMaxLen; iMove++)
	{
		if (Text[iMove] == '\0')
		{
			//绘制当前行，并退出。
			SIZE sz;
			GetTextExtentPoint32(hdc, Text + LastMove, iMove - LastMove, &sz);
			//TextOut(wParam, 0, yCount, Text + LastMove, iMove - LastMove);
			//yCount += tm.tmHeight;
			xCount = sz.cx;
			break;
		}
		else if (Text[iMove] == '\r' && Text[iMove + 1] == '\n')
		{
			//windows换行标记，绘制当前行，重新开始。
			//TextOut(wParam, 0, yCount, Text + LastMove, iMove - LastMove);
			yCount += tm.tmHeight;
			LastMove = iMove + 2;
			iMove++;
		}
		else if (Text[iMove] == '\n')
		{
			//Linux换行标记，绘制当前行。
			//TextOut(hdc, 0, yCount, Text + LastMove, iMove - LastMove);
			yCount += tm.tmHeight;
			LastMove = iMove + 1;
		}
		else
		{
			//检查一下会不会长度超出窗口宽度
			//窗口过窄有bug
			SIZE sz, sz2;
			GetTextExtentPoint32(hdc, Text + LastMove, iMove - LastMove, &sz);
			GetTextExtentPoint32(hdc, Text + LastMove, iMove - LastMove + 1, &sz2);
			if (sz.cx <= Width && sz2.cx > Width)
			{
				//截断
				//TextOut(wParam, 0, yCount, Text + LastMove, iMove - LastMove);
				yCount += tm.tmHeight;
				LastMove = iMove;// +1;
			}
		}
	}


	//往后推CaretPos个字符


	LastMove = 0;
	for (iMove = 0; iMove <= CaretPos; iMove++)
	{
		if (InputText[iMove] == '\0' || iMove == CaretPos)
		{
			//绘制当前行，并退出。
			SIZE sz;
			//TextOut(wParam, xCount, yCount, InputText + LastMove, iMove - LastMove);
			GetTextExtentPoint32(hdc, InputText + LastMove, iMove - LastMove, &sz);
			//TextOut(wParam, 0, yCount, Text + LastMove, iMove - LastMove);
			//yCount += tm.tmHeight;
			xCount += sz.cx;
			//xCount = 0;
			//yCount += tm.tmHeight;
			break;
		}
		else
		{
			//检查一下会不会长度超出窗口宽度
			SIZE sz, sz2;
			GetTextExtentPoint32(hdc, InputText + LastMove, iMove - LastMove + 1, &sz);
			GetTextExtentPoint32(hdc, InputText + LastMove, iMove - LastMove + 2, &sz2);
			if (sz.cx + xCount <= Width && sz2.cx + xCount > Width)
			{
				//截断
				//TextOut(wParam, xCount, yCount, InputText + LastMove, iMove - LastMove);
				xCount = 0;
				yCount += tm.tmHeight;
				LastMove = iMove + 1;
			}
		}
	}


	pt->x = xCount;
	pt->y = yCount + tm.tmHeight - 4;
	SelectFont(hdc, OldFont);
	return TRUE;
}


pVTEXT InitVText()
{
	pVTEXT pVText = malloc(sizeof(VTEXT));
	memset(pVText, 0, sizeof(VTEXT));
	return pVText;
}

BOOL FreeVText(pVTEXT pVText)
{
	if (pVText->Capibility)
	{
		free(pVText->Text);
	}
	free(pVText);
}

BOOL SetVText(pVTEXT pVText, TCHAR Str[], int len)//len=-1时自动获取长度
{
	if (len == -1)
	{
		len = lstrlen(Str);
	}

	//找len的最高位
	pVText->Capibility = 0;
	for (int bit = 1 << 15; bit > 0; bit >>= 1)
	{
		if (bit & len)
		{
			//找到了。
			pVText->Capibility = bit << 1;
			break;
		}
	}

	if (pVText->Capibility == 0)
	{
		if (pVText->Text)
		{
			free(pVText->Text);
			pVText->Text = 0;
		}
	}
	else
	{
		if (pVText->Text)
		{
			pVText->Text = realloc(pVText->Text, pVText->Capibility * sizeof(TCHAR));
		}
		else
		{
			pVText->Text = malloc(pVText->Capibility * sizeof(TCHAR));
		}
		lstrcpy(pVText->Text, Str);
		pVText->Text[len] = 0;
	}
	pVText->Length = len;


	return TRUE;
}


BOOL VTextInsertChar(pVTEXT pVText, TCHAR ch, int InsertBefore)
{
	pVText->Length++;


	for (int bit = 1 << 15; bit > 0; bit >>= 1)
	{
		if (bit & pVText->Length)
		{
			//找到了。
			pVText->Capibility = bit << 1;
			break;
		}
	}


	if (pVText->Text)
	{
		pVText->Text = realloc(pVText->Text, pVText->Capibility * sizeof(TCHAR));
	}
	else
	{
		pVText->Text = malloc(pVText->Capibility * sizeof(TCHAR));
	}

	//内容后移

	memmove(pVText->Text + InsertBefore + 1, pVText->Text + InsertBefore, (pVText->Length - 1 - InsertBefore) * sizeof(TCHAR));
	pVText->Text[pVText->Length] = 0;
	pVText->Text[InsertBefore] = ch;

	return TRUE;
}

