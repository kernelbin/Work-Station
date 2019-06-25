#include<Windows.h>
#include"VText.h"


pVTEXT InitVText()
{
	pVTEXT pVText = malloc(sizeof(VTEXT));
	if (!pVText) return 0;
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

BOOL CatVText(pVTEXT pVTextA, pVTEXT pVTextB)
{
	//��B����A����

	int OrgALen = pVTextA->Length;
	pVTextA->Length += pVTextB->Length;


	pVTextA->Capibility = 0;
	for (int bit = 1 << 15; bit > 0; bit >>= 1)
	{
		if (bit & pVTextA->Length)
		{
			//�ҵ��ˡ�
			pVTextA->Capibility = bit << 1;
			break;
		}
	}

	if (pVTextA->Capibility == 0)
	{
		//����A��B���ȶ���0��ɶ��������
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