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
	//�������������ط����������ã�ʹ�ù̶�����
	static int CaretPos = 0;//�ڼ����ַ���


	static pVTEXT Text;
	static pVTEXT InputText;
	//static TCHAR Text[] = TEXT("Microsoft Windows [�汾 10.0.18362.175]\r\n(c)2019 Microsoft Corporation����������Ȩ����\r\n\r\nC:\\Users\\11603>");//ezWnd->ezParent->ezParent->Extend->Title;

	//static TCHAR InputText[] = TEXT("sudo apt-get install minecraft");//ezWnd->ezParent->ezParent->Extend->Title;

	//static int InputLen = 20;

	POINT pt;



	switch (message)
	{
	case EZWM_CREATE:
		ConsoleFont = CreateFont(16, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("������"));

		Text = InitVText();
		InputText = InitVText();
		SetVText(Text, TEXT("Workstation C Shell Console [�汾 0.0.1]\r\n(c)2019 ��ա���������Ȩ����\r\n\r\nC:\\Users\\11603>\r\n\r\n"), -1);
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

		TEXTMETRIC tm;
		GetTextMetrics(wParam, &tm);
		for (iMove = 0; iMove <= iMaxLen; iMove++)
		{
			if (Text->Text[iMove] == '\0')
			{
				//���Ƶ�ǰ�У����˳���
				//SIZE sz;
				//GetTextExtentPoint32(wParam, Text->Text + LastMove, iMove - LastMove, &sz);
				TextOut(wParam, 0, yCount, Text->Text + LastMove, iMove - LastMove);
				//yCount += tm.tmHeight;
				//xCount = sz.cx;
				break;
			}
			else if (Text->Text[iMove] == '\r' && Text->Text[iMove + 1] == '\n')
			{
				//windows���б�ǣ����Ƶ�ǰ�У����¿�ʼ��
				TextOut(wParam, 0, yCount, Text->Text + LastMove, iMove - LastMove);
				xCount = 0;
				yCount += tm.tmHeight;
				LastMove = iMove + 2;
				iMove++;
			}
			else if (Text->Text[iMove] == '\n')
			{
				//Linux���б�ǣ����Ƶ�ǰ�С�
				TextOut(wParam, 0, yCount, Text->Text + LastMove, iMove - LastMove);
				xCount = 0;
				yCount += tm.tmHeight;
				LastMove = iMove + 1;
			}
			else
			{
				//���һ�»᲻�᳤�ȳ������ڿ��
				//SIZE sz, sz2;
				//GetTextExtentPoint32(wParam, Text->Text + LastMove, iMove - LastMove, &sz);
				//GetTextExtentPoint32(wParam, Text->Text + LastMove, iMove - LastMove + 1, &sz2);
				//if (sz.cx <= ezWnd->Width && sz2.cx > ezWnd->Width)
				//{
				//	//�ض�
				//	TextOut(wParam, 0, yCount, Text->Text + LastMove, iMove - LastMove);
				//	yCount += tm.tmHeight;
				//	LastMove = iMove;// +1;
				//}


				int len;
				GetCharWidth32(wParam, *(Text->Text + LastMove), *(Text->Text + LastMove), &len);
				//TextOut(wParam, 0, yCount, Text + LastMove, iMove - LastMove);
				//yCount += tm.tmHeight;
				

				if (xCount <= ezWnd->Width && xCount +len > ezWnd->Width)
				{
					//�ض�
					//TextOut(wParam, 0, yCount, Text + LastMove, iMove - LastMove);
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
		int xBegin = xCount;
		for (iMove = 0; iMove <= iMaxLen; iMove++)
		{
			if (InputText->Text[iMove] == '\0')
			{
				//���Ƶ�ǰ�У����˳���
				TextOut(wParam, xBegin, yCount, InputText->Text + LastMove, iMove - LastMove);
				xBegin = 0;
				xCount = 0;
				yCount += tm.tmHeight;
				break;
			}
			else if (InputText->Text[iMove] == '\r' && InputText->Text[iMove + 1] == '\n')
			{
				//windows���б�ǣ����Ƶ�ǰ�У����¿�ʼ��
				TextOut(wParam, xBegin, yCount, InputText->Text + LastMove, iMove - LastMove);
				xBegin = 0;
				xCount = 0;
				yCount += tm.tmHeight;
				LastMove = iMove + 2;
				iMove++;
			}
			else if (InputText->Text[iMove] == '\n')
			{
				//Linux���б�ǣ����Ƶ�ǰ�С�
				TextOut(wParam, xBegin, yCount, InputText->Text + LastMove, iMove - LastMove);
				xBegin = 0;
				xCount = 0;
				yCount += tm.tmHeight;
				LastMove = iMove + 1;
			}
			else
			{
				//���һ�»᲻�᳤�ȳ������ڿ��
				//SIZE sz, sz2;
				//GetTextExtentPoint32(wParam, InputText->Text + LastMove, iMove - LastMove, &sz);
				//GetTextExtentPoint32(wParam, InputText->Text + LastMove, iMove - LastMove + 1, &sz2);
				//if (sz.cx + xCount <= ezWnd->Width && sz2.cx + xCount > ezWnd->Width)
				//{
				//	//�ض�
				//	TextOut(wParam, xCount, yCount, InputText->Text + LastMove, iMove - LastMove);
				//	xCount = 0;
				//	yCount += tm.tmHeight;
				//	LastMove = iMove;// +1;
				//}


				//���һ�»᲻�᳤�ȳ������ڿ��
				int len;
				GetCharWidth32(wParam, *(InputText->Text + iMove), *(InputText->Text + iMove), &len);
				//GetCharWidth32(wParam, *(InputText->Text + LastMove + 1), *(InputText->Text + LastMove + 1), &lennext);
				//TextOut(wParam, 0, yCount, Text + LastMove, iMove - LastMove);
				//yCount += tm.tmHeight;
				

				if (xCount <= ezWnd->Width && xCount + len > ezWnd->Width)
				{
					//�ض�
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
		//�ַ���Ϣ

		switch (wParam)
		{
		case 3:
			// Ctrl + C

			break;

		case '\n':
			Sleep(0);
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

		
		LocateCaretPos(ezWnd->hdc, ConsoleFont, Text->Text, InputText->Text, ezWnd->Width, CaretPos, &pt);
		EZSetCaretPos(ezWnd, pt.x, pt.y);
		EZRepaint(ezWnd, 0);

		EZShowCaret(ezWnd);

		return 0;
		
	case EZWM_KEYDOWN:
		//�����������Ҽ��������л����룬�����л���ʷָ�
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
			//����ɾ����
			if (CaretPos == InputText->Length)
			{
				MessageBeep(0);
			}
			else
			{
				VTextDeleteChar(InputText, CaretPos + 1);

				EZHideCaret(ezWnd);


				LocateCaretPos(ezWnd->hdc, ConsoleFont, Text->Text, InputText->Text, ezWnd->Width, CaretPos, &pt);
				EZSetCaretPos(ezWnd, pt.x, pt.y);
				EZRepaint(ezWnd, 0);

				EZShowCaret(ezWnd);

			}
			break;
		/*case VK_RETURN:
		{
			USHORT ShiftState = GetKeyState(VK_SHIFT);
			MessageBeep(0);
		}
			*/
			
			//��Shift��
			break;
		}
		

		
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
		//TODO:��ȷ�����ı��߶�+���ڸ߶ȣ�������
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
	//�����λ�ò�����
	HFONT OldFont = SelectObject(hdc, hFont);


	int iMaxLen = lstrlen(Text);
	int LastMove = 0;
	int xCount = 0;
	int yCount = 0;
	int iMove;


	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);

	if (Text)
	{
		for (iMove = 0; iMove <= iMaxLen; iMove++)
		{
			if (Text[iMove] == '\0')
			{
				//���Ƶ�ǰ�У����˳���
				
				break;
			}
			else if (Text[iMove] == '\r' && Text[iMove + 1] == '\n')
			{
				//windows���б�ǣ����Ƶ�ǰ�У����¿�ʼ��
				//TextOut(wParam, 0, yCount, Text + LastMove, iMove - LastMove);
				xCount = 0;
				yCount += tm.tmHeight;
				LastMove = iMove + 2;
				iMove++;
			}
			else if (Text[iMove] == '\n')
			{
				//Linux���б�ǣ����Ƶ�ǰ�С�
				//TextOut(hdc, 0, yCount, Text + LastMove, iMove - LastMove);
				xCount = 0;
				yCount += tm.tmHeight;
				LastMove = iMove + 1;
			}
			else
			{
				//���һ�»᲻�᳤�ȳ������ڿ��
				//���ڹ�խ��bug
				//SIZE sz, sz2;
				//GetTextExtentPoint32(hdc, Text + LastMove, iMove - LastMove, &sz);
				//GetTextExtentPoint32(hdc, Text + LastMove, iMove - LastMove + 1, &sz2);

				int len;
				GetCharWidth32(hdc, *(Text + LastMove), *(Text + LastMove), &len);
				
				//TextOut(wParam, 0, yCount, Text + LastMove, iMove - LastMove);
				//yCount += tm.tmHeight;
				

				if (xCount <= Width && xCount + len > Width)
				{
					//�ض�
					//TextOut(wParam, 0, yCount, Text + LastMove, iMove - LastMove);
					yCount += tm.tmHeight;
					LastMove = iMove;// +1;
					xCount = 0;
				}
				else
				{
					xCount += len;
				}
			}
		}
	}
	


	//������CaretPos���ַ�


	if (InputText)
	{
		LastMove = 0;
		for (iMove = 0; iMove <= CaretPos; iMove++)
		{
			if (InputText[iMove] == '\0' || iMove == CaretPos)
			{
				//���Ƶ�ǰ�У����˳���
				break;
			}
			else if (InputText[iMove] == '\r' && InputText[iMove + 1] == '\n')
			{
				//windows���б�ǣ����Ƶ�ǰ�У����¿�ʼ��
				//TextOut(hdc, xCount, yCount, InputText + LastMove, iMove - LastMove);
				xCount = 0;
				yCount += tm.tmHeight;
				LastMove = iMove + 2;
				iMove++;
			}
			else if (InputText[iMove] == '\n')
			{
				//Linux���б�ǣ����Ƶ�ǰ�С�
				xCount = 0;
				yCount += tm.tmHeight;
				LastMove = iMove + 1;
			}
			else
			{
				//���һ�»᲻�᳤�ȳ������ڿ��
				int len;
				GetCharWidth32(hdc, *(InputText + iMove), *(InputText + iMove), &len);
				
				//TextOut(wParam, 0, yCount, Text + LastMove, iMove - LastMove);
				//yCount += tm.tmHeight;
				

				if (xCount <= Width && xCount + len > Width)
				{
					//�ض�
					//TextOut(wParam, 0, yCount, Text + LastMove, iMove - LastMove);
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
}

BOOL SetVText(pVTEXT pVText, TCHAR Str[], int len)//len=-1ʱ�Զ���ȡ����
{
	if (len == -1)
	{
		len = lstrlen(Str);
	}

	//��len�����λ
	pVText->Capibility = 0;
	for (int bit = 1 << 15; bit > 0; bit >>= 1)
	{
		if (bit & len)
		{
			//�ҵ��ˡ�
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
			//�ҵ��ˡ�
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

	//���ݺ���

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

	//����ǰ��

	memmove(pVText->Text + DeleteBefore - 1, pVText->Text + DeleteBefore, pVText->Length - DeleteBefore);


	pVText->Length--;

	if (pVText->Length)
	{
		for (int bit = 1 << 15; bit > 0; bit >>= 1)
		{
			if (bit & pVText->Length)
			{
				//�ҵ��ˡ�
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
	return TRUE;
}

