#include<Windows.h>
#include"EasyWindow.h"
#include"Global.h"


typedef struct __tagVectorText
{
	int Capibility;
	int Length;
	TCHAR* Text;
}VECTORTEXT, VTEXT, * pVTEXT;


//BOOL LocateCaretPos(HDC hdc, HFONT hFont, TCHAR Text[], TCHAR InputText[], int Width, int CaretPos, PPOINT pt);
BOOL LocateCaretPos(HDC hdc, HFONT hFont, pVTEXT Text, pVTEXT InputText, int Width, int CaretPos, PPOINT pt);

pVTEXT InitVText();
BOOL FreeVText(pVTEXT pVText);
BOOL ClearVText(pVTEXT pVText);
BOOL SetVText(pVTEXT pVText, TCHAR Str[], int len);

BOOL CommandHandler(pVTEXT Command);


EZWNDPROC ConsolePageProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	static HFONT ConsoleFont;
	//这里无视其他地方的字体设置，使用固定字体
	static int CaretPos = 0;//第几个字符。


	static pVTEXT Text;
	static pVTEXT InputText;
	

	POINT pt;



	switch (message)
	{
	case EZWM_CREATE:
		ConsoleFont = CreateFont(16, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("新宋体"));

		Text = InitVText();
		InputText = InitVText();
		SetVText(Text, TEXT("Workstation C Shell Console [版本 0.0.1]\n(c)2019 yh。保留所有权利。\n\nC:\\Users\\11603>\n\n"), -1);
		//SetVText(InputText, TEXT("sudo apt-get install minecraft"), -1);
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
		int iMaxLen = Text->Length;
		int LastMove = 0;
		int xCount = 0;
		int yCount = 0;
		int iMove;
		int xBegin = 0;



		TEXTMETRIC tm;
		GetTextMetrics(wParam, &tm);
		for (iMove = 0; iMove <= iMaxLen; iMove++)
		{
			if (Text->Text[iMove] == '\0')
			{
				//绘制当前行，并退出。
				//SIZE sz;
				//GetTextExtentPoint32(wParam, Text->Text + LastMove, iMove - LastMove, &sz);
				TextOut(wParam, xBegin, yCount, Text->Text + LastMove, iMove - LastMove);
				//yCount += tm.tmHeight;
				//xCount = sz.cx;
				break;
			}
			else if (Text->Text[iMove] == '\r' && Text->Text[iMove + 1] == '\n')
			{
				//windows换行标记，绘制当前行，重新开始。
				TextOut(wParam, xBegin, yCount, Text->Text + LastMove, iMove - LastMove);
				xBegin = 0;
				xCount = 0;
				yCount += tm.tmHeight;
				LastMove = iMove + 2;
				iMove++;
			}
			else if (Text->Text[iMove] == '\n')
			{
				//Linux换行标记，绘制当前行。
				TextOut(wParam, xBegin, yCount, Text->Text + LastMove, iMove - LastMove);
				xBegin = 0;
				xCount = 0;
				yCount += tm.tmHeight;
				LastMove = iMove + 1;
			}
			else
			{
				//检查一下会不会长度超出窗口宽度
				//SIZE sz, sz2;
				//GetTextExtentPoint32(wParam, Text->Text + LastMove, iMove - LastMove, &sz);
				//GetTextExtentPoint32(wParam, Text->Text + LastMove, iMove - LastMove + 1, &sz2);
				//if (sz.cx <= ezWnd->Width && sz2.cx > ezWnd->Width)
				//{
				//	//截断
				//	TextOut(wParam, 0, yCount, Text->Text + LastMove, iMove - LastMove);
				//	yCount += tm.tmHeight;
				//	LastMove = iMove;// +1;
				//}



				//检查一下会不会长度超出窗口宽度
				int len;
				if (*(Text->Text + iMove) == '\t')
				{
					//制表符，特殊处理。先把前面的解决掉
					TextOut(wParam, xBegin, yCount, Text->Text + LastMove, iMove - LastMove);
					xBegin = xCount;
					LastMove = iMove + 1;

					//顶到16的倍数
					len = 32 - xCount % 32;

					xBegin += len;

				}
				else
				{
					GetCharWidth32(wParam, *(Text->Text + iMove), *(Text->Text + iMove), &len);
				}

				//GetCharWidth32(wParam, *(InputText->Text + LastMove + 1), *(InputText->Text + LastMove + 1), &lennext);
				//TextOut(wParam, 0, yCount, Text + LastMove, iMove - LastMove);
				//yCount += tm.tmHeight;


				if (xCount <= ezWnd->Width && xCount + len > ezWnd->Width)
				{
					//截断
					TextOut(wParam, xBegin, yCount, Text->Text + LastMove, iMove - LastMove);
					xBegin = 0;
					yCount += tm.tmHeight;
					LastMove = iMove;// +1;
					xCount = len;
				}
				else
				{
					xCount += len;
				}

			}
		}


		SetTextColor(wParam, RGB(255, 255, 255));

		if (!InputText->Text)
		{
			return 0;
		}
		iMaxLen = InputText->Length;
		LastMove = 0;
		xBegin = xCount;
		for (iMove = 0; iMove <= iMaxLen; iMove++)
		{
			if (InputText->Text[iMove] == '\0')
			{
				//绘制当前行，并退出。
				TextOut(wParam, xBegin, yCount, InputText->Text + LastMove, iMove - LastMove);
				xBegin = 0;
				xCount = 0;
				yCount += tm.tmHeight;
				break;
			}
			else if (InputText->Text[iMove] == '\r' && InputText->Text[iMove + 1] == '\n')
			{
				//windows换行标记，绘制当前行，重新开始。
				TextOut(wParam, xBegin, yCount, InputText->Text + LastMove, iMove - LastMove);
				xBegin = 0;
				xCount = 0;
				yCount += tm.tmHeight;
				LastMove = iMove + 2;
				iMove++;
			}
			else if (InputText->Text[iMove] == '\n')
			{
				//Linux换行标记，绘制当前行。
				TextOut(wParam, xBegin, yCount, InputText->Text + LastMove, iMove - LastMove);
				xBegin = 0;
				xCount = 0;
				yCount += tm.tmHeight;
				LastMove = iMove + 1;
			}
			else
			{
				//检查一下会不会长度超出窗口宽度
				//SIZE sz, sz2;
				//GetTextExtentPoint32(wParam, InputText->Text + LastMove, iMove - LastMove, &sz);
				//GetTextExtentPoint32(wParam, InputText->Text + LastMove, iMove - LastMove + 1, &sz2);
				//if (sz.cx + xCount <= ezWnd->Width && sz2.cx + xCount > ezWnd->Width)
				//{
				//	//截断
				//	TextOut(wParam, xCount, yCount, InputText->Text + LastMove, iMove - LastMove);
				//	xCount = 0;
				//	yCount += tm.tmHeight;
				//	LastMove = iMove;// +1;
				//}


				//检查一下会不会长度超出窗口宽度
				int len;
				if (*(InputText->Text + iMove) == '\t')
				{
					//制表符，特殊处理。先把前面的解决掉
					TextOut(wParam, xBegin, yCount, InputText->Text + LastMove, iMove - LastMove);
					xBegin = xCount;
					LastMove = iMove+1;

					//顶到16的倍数
					len = 32 - xCount % 32;

					xBegin += len;

				}
				else
				{
					GetCharWidth32(wParam, *(InputText->Text + iMove), *(InputText->Text + iMove), &len);
				}
				
				//GetCharWidth32(wParam, *(InputText->Text + LastMove + 1), *(InputText->Text + LastMove + 1), &lennext);
				//TextOut(wParam, 0, yCount, Text + LastMove, iMove - LastMove);
				//yCount += tm.tmHeight;
				

				if (xCount <= ezWnd->Width && xCount + len > ezWnd->Width)
				{
					//截断
					TextOut(wParam, xBegin, yCount, InputText->Text + LastMove, iMove - LastMove);
					xBegin = 0;
					yCount += tm.tmHeight;
					LastMove = iMove;// +1;
					xCount = len;
				}
				else
				{
					xCount += len;
				}

			}
		}
	}
	return 0;


	case EZWM_CHAR:
		//字符消息

		switch (wParam)
		{
		case 3:
			// Ctrl + C

			break;

		case '\n':
			CommandHandler(InputText);
			//将文本并入
			CatVText(Text, InputText);

			VTextInsertChar(Text, '\n', Text->Length);

			ClearVText(InputText);
			CaretPos = 0;

			break;

		case '\r':
			//VTextInsertChar(InputText, '\r', CaretPos);
			VTextInsertChar(InputText, '\n', CaretPos);
			CaretPos++;
			break;

		case '\b':
			if (CaretPos == 0)
			{
				MessageBeep(0);
			}
			else
			{
				VTextDeleteChar(InputText, CaretPos);
				CaretPos--;
			}
			break;

		default:
			VTextInsertChar(InputText, wParam, CaretPos);
			CaretPos++;

			break;
		}

		EZHideCaret(ezWnd);

		
		LocateCaretPos(ezWnd->hdc, ConsoleFont, Text, InputText, ezWnd->Width, CaretPos, &pt);
		EZSetCaretPos(ezWnd, pt.x, pt.y);
		EZRepaint(ezWnd, 0);

		EZShowCaret(ezWnd);

		return 0;
		
	case EZWM_KEYDOWN:
		//处理上下左右键（左右切换输入，上下切换历史指令）
		switch (wParam)
		{
		case VK_LEFT:
			if (CaretPos > 0)
			{
				CaretPos--;
			}
			else
			{
				MessageBeep(0);
			}
			break;
		case VK_RIGHT:
			if (CaretPos < InputText->Length)
			{
				CaretPos++;
			}
			else
			{
				MessageBeep(0);
			}
			break;
		case VK_DELETE:
			//处理删除键
			if (CaretPos == InputText->Length)
			{
				MessageBeep(0);
			}
			else
			{
				VTextDeleteChar(InputText, CaretPos + 1);

				EZHideCaret(ezWnd);


				LocateCaretPos(ezWnd->hdc, ConsoleFont, Text, InputText, ezWnd->Width, CaretPos, &pt);
				EZSetCaretPos(ezWnd, pt.x, pt.y);
				EZRepaint(ezWnd, 0);

				EZShowCaret(ezWnd);

			}
			break;
		}
		

		
		LocateCaretPos(ezWnd->hdc, ConsoleFont, Text, InputText, ezWnd->Width, CaretPos, &pt);
		EZSetCaretPos(ezWnd, pt.x, pt.y);

		return 0;

	case EZWM_SETFOCUS:

		EZCreateCaret(ezWnd, 0, 8, 4);

		//EZSetCaretPos(ezWnd, 0, 0);
		{

			POINT pt;
			LocateCaretPos(ezWnd->hdc, ConsoleFont, Text, InputText, ezWnd->Width, CaretPos, &pt);
			EZSetCaretPos(ezWnd, pt.x, pt.y);
		}
		EZShowCaret(ezWnd);
		return 0;

	case EZWM_KILLFOCUS:

		EZHideCaret(ezWnd);
		EZDestroyCaret();

		return 0;
	case EZWM_USER_NOTIFY:
		//TODO:正确计算文本高度+窗口高度，并返回
		return 1200;

	case EZWM_DESTROY:
		DeleteObject(ConsoleFont);
		FreeVText(InputText);
		FreeVText(Text);
	}
	return 0;
}

BOOL LocateCaretPos(HDC hdc, HFONT hFont, pVTEXT Text, pVTEXT InputText, int Width, int CaretPos, PPOINT pt)
{
	//计算出位置并返回
	HFONT OldFont = SelectObject(hdc, hFont);

	if (!Text->Text)
	{
		return 0;
	}
	int iMaxLen = Text->Length;
	int LastMove = 0;
	int xCount = 0;
	int yCount = 0;
	int iMove;
	int xBegin = 0;



	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);
	for (iMove = 0; iMove <= iMaxLen; iMove++)
	{
		if (Text->Text[iMove] == '\0')
		{
			//绘制当前行，并退出。
			//SIZE sz;
			//GetTextExtentPoint32(wParam, Text->Text + LastMove, iMove - LastMove, &sz);
			//TextOut(hdc, xBegin, yCount, Text->Text + LastMove, iMove - LastMove);
			//yCount += tm.tmHeight;
			//xCount = sz.cx;
			break;
		}
		else if (Text->Text[iMove] == '\r' && Text->Text[iMove + 1] == '\n')
		{
			//windows换行标记，绘制当前行，重新开始。
			//TextOut(hdc, xBegin, yCount, Text->Text + LastMove, iMove - LastMove);
			xBegin = 0;
			xCount = 0;
			yCount += tm.tmHeight;
			LastMove = iMove + 2;
			iMove++;
		}
		else if (Text->Text[iMove] == '\n')
		{
			//Linux换行标记，绘制当前行。
			//TextOut(wParam, xBegin, yCount, Text->Text + LastMove, iMove - LastMove);
			xBegin = 0;
			xCount = 0;
			yCount += tm.tmHeight;
			LastMove = iMove + 1;
		}
		else
		{
			//检查一下会不会长度超出窗口宽度
			//SIZE sz, sz2;
			//GetTextExtentPoint32(wParam, Text->Text + LastMove, iMove - LastMove, &sz);
			//GetTextExtentPoint32(wParam, Text->Text + LastMove, iMove - LastMove + 1, &sz2);
			//if (sz.cx <= ezWnd->Width && sz2.cx > ezWnd->Width)
			//{
			//	//截断
			//	TextOut(wParam, 0, yCount, Text->Text + LastMove, iMove - LastMove);
			//	yCount += tm.tmHeight;
			//	LastMove = iMove;// +1;
			//}


			int len;
			if (*(Text->Text + iMove) == '\t')
			{
				//制表符，特殊处理。先把前面的解决掉
				TextOut(hdc, xBegin, yCount, Text->Text + LastMove, iMove - LastMove);
				xBegin = xCount;
				LastMove = iMove + 1;

				//顶到16的倍数
				len = 32 - xCount % 32;

				xBegin += len;

			}
			else
			{
				GetCharWidth32(hdc, *(Text->Text + iMove), *(Text->Text + iMove), &len);
			}

			//GetCharWidth32(wParam, *(InputText->Text + LastMove + 1), *(InputText->Text + LastMove + 1), &lennext);
			//TextOut(wParam, 0, yCount, Text + LastMove, iMove - LastMove);
			//yCount += tm.tmHeight;


			if (xCount <= Width && xCount + len > Width)
			{
				//截断
				TextOut(hdc, xBegin, yCount, Text->Text + LastMove, iMove - LastMove);
				xBegin = 0;
				yCount += tm.tmHeight;
				LastMove = iMove;// +1;
				xCount = len;
			}
			else
			{
				xCount += len;
			}
		}
	}


	if (!InputText->Text)
	{
		if (xCount + 8 > Width)
		{
			yCount += tm.tmHeight;
			xCount = 0;
		}
		pt->x = xCount;
		pt->y = yCount + tm.tmHeight - 4;
		SelectFont(hdc, OldFont);
		return TRUE;
	}
	iMaxLen = InputText->Length;
	LastMove = 0;
	xBegin = xCount;
	for (iMove = 0; iMove < CaretPos; iMove++)
	{
		if (InputText->Text[iMove] == '\0')
		{
			//绘制当前行，并退出。
			//TextOut(hdc, xBegin, yCount, InputText->Text + LastMove, iMove - LastMove);
			break;
		}
		else if (InputText->Text[iMove] == '\r' && InputText->Text[iMove + 1] == '\n')
		{
			//windows换行标记，绘制当前行，重新开始。
			//TextOut(wParam, xBegin, yCount, InputText->Text + LastMove, iMove - LastMove);
			xBegin = 0;
			xCount = 0;
			yCount += tm.tmHeight;
			LastMove = iMove + 2;
			iMove++;
		}
		else if (InputText->Text[iMove] == '\n')
		{
			//Linux换行标记，绘制当前行。
			//TextOut(wParam, xBegin, yCount, InputText->Text + LastMove, iMove - LastMove);
			xBegin = 0;
			xCount = 0;
			yCount += tm.tmHeight;
			LastMove = iMove + 1;
		}
		else
		{
			//检查一下会不会长度超出窗口宽度
			//SIZE sz, sz2;
			//GetTextExtentPoint32(wParam, InputText->Text + LastMove, iMove - LastMove, &sz);
			//GetTextExtentPoint32(wParam, InputText->Text + LastMove, iMove - LastMove + 1, &sz2);
			//if (sz.cx + xCount <= ezWnd->Width && sz2.cx + xCount > ezWnd->Width)
			//{
			//	//截断
			//	TextOut(wParam, xCount, yCount, InputText->Text + LastMove, iMove - LastMove);
			//	xCount = 0;
			//	yCount += tm.tmHeight;
			//	LastMove = iMove;// +1;
			//}


			//检查一下会不会长度超出窗口宽度
			int len;
			if (*(InputText->Text + iMove) == '\t')
			{
				//制表符，特殊处理。先把前面的解决掉
				//TextOut(wParam, xBegin, yCount, InputText->Text + LastMove, iMove - LastMove);
				xBegin = xCount;
				LastMove = iMove + 1;

				//顶到16的倍数
				len = 32 - xCount % 32;

				xBegin += len;

			}
			else
			{
				GetCharWidth32(hdc, *(InputText->Text + iMove), *(InputText->Text + iMove), &len);
			}

			//GetCharWidth32(wParam, *(InputText->Text + LastMove + 1), *(InputText->Text + LastMove + 1), &lennext);
			//TextOut(wParam, 0, yCount, Text + LastMove, iMove - LastMove);
			//yCount += tm.tmHeight;


			if (xCount <= Width && xCount + len > Width)
			{
				//截断
				//TextOut(wParam, xBegin, yCount, InputText->Text + LastMove, iMove - LastMove);
				xBegin = 0;
				yCount += tm.tmHeight;
				LastMove = iMove;
				xCount = len;
			}
			else
			{
				xCount += len;
			}

		}
	}
	if (xCount + 8 > Width)
	{
		yCount += tm.tmHeight;
		xCount = 0;
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
	return TRUE;
}

BOOL ClearVText(pVTEXT pVText)
{
	if (pVText->Capibility)
	{
		free(pVText->Text);
		pVText->Text = 0;
		pVText->Length = 0;
		pVText->Capibility = 0;
	}
	return TRUE;
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

BOOL CatVText(pVTEXT pVTextA, pVTEXT pVTextB)
{
	//把B接在A后面

	int OrgALen = pVTextA->Length;
	pVTextA->Length += pVTextB->Length;


	pVTextA->Capibility = 0;
	for (int bit = 1 << 15; bit > 0; bit >>= 1)
	{
		if (bit & pVTextA->Length)
		{
			//找到了。
			pVTextA->Capibility = bit << 1;
			break;
		}
	}

	if (pVTextA->Capibility == 0)
	{
		//看来A和B长度都是0，啥都不用做
		return TRUE;
	}
	else
	{
		if (pVTextA->Text)
		{
			pVTextA->Text = realloc(pVTextA->Text, pVTextA->Capibility * sizeof(TCHAR));
		}
		else
		{
			pVTextA->Text = malloc(pVTextA->Capibility * sizeof(TCHAR));
		}

		if (pVTextB->Text)
		{
			lstrcpy(pVTextA->Text + OrgALen, pVTextB->Text);
			pVTextA->Text[pVTextA->Length] = 0;
		}
	}

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



BOOL VTextDeleteChar(pVTEXT pVText, int DeleteBefore)
{
	if (!pVText->Text)
	{
		return 0;
	}

	if (DeleteBefore == 0)
	{
		return 0;
	}

	//内容前移

	memmove(pVText->Text + DeleteBefore - 1, pVText->Text + DeleteBefore, (pVText->Length - DeleteBefore) * sizeof(TCHAR));


	pVText->Length--;

	if (pVText->Length)
	{
		for (int bit = 1 << 15; bit > 0; bit >>= 1)
		{
			if (bit & pVText->Length)
			{
				//找到了。
				pVText->Capibility = bit << 1;
				break;
			}
		}
		pVText->Text = realloc(pVText->Text, pVText->Capibility * sizeof(TCHAR));
		pVText->Text[pVText->Length] = 0;
	}
	else
	{
		free(pVText->Text);
		pVText->Text = 0;
		pVText->Capibility = 0;
	}
	

	

	return TRUE;
}


BOOL CommandHandler(pVTEXT Command)
{
	MessageBox(0, Command->Text, TEXT("command"), 0);
	return TRUE;
}

