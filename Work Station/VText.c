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




BOOL SetVTextA(pVTEXTA pVText, char Str[], int len)
{
	if (len == -1)
	{
		len = lstrlenA(Str);
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
			pVText->Text = realloc(pVText->Text, pVText->Capibility * sizeof(char));
		}
		else
		{
			pVText->Text = malloc(pVText->Capibility * sizeof(char));
		}
		lstrcpyA(pVText->Text, Str);
		pVText->Text[len] = 0;
	}
	pVText->Length = len;


	return TRUE;
}

BOOL SetVTextW(pVTEXTW pVText, WCHAR Str[], int len)
{
	if (len == -1)
	{
		len = lstrlenW(Str);
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
			pVText->Text = realloc(pVText->Text, pVText->Capibility * sizeof(WCHAR));
		}
		else
		{
			pVText->Text = malloc(pVText->Capibility * sizeof(WCHAR));
		}
		lstrcpyW(pVText->Text, Str);
		pVText->Text[len] = 0;
	}
	pVText->Length = len;


	return TRUE;
}


BOOL CatVTextA(pVTEXTA pVTextA, pVTEXTA pVTextB)
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
			pVTextA->Text = realloc(pVTextA->Text, pVTextA->Capibility * sizeof(char));
		}
		else
		{
			pVTextA->Text = malloc(pVTextA->Capibility * sizeof(char));
		}

		if (pVTextB->Text)
		{
			lstrcpyA(pVTextA->Text + OrgALen, pVTextB->Text);
			pVTextA->Text[pVTextA->Length] = 0;
		}
	}

	return TRUE;
}


BOOL CatVTextW(pVTEXTW pVTextA, pVTEXTW pVTextB)
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
			pVTextA->Text = realloc(pVTextA->Text, pVTextA->Capibility * sizeof(WCHAR));
		}
		else
		{
			pVTextA->Text = malloc(pVTextA->Capibility * sizeof(WCHAR));
		}

		if (pVTextB->Text)
		{
			lstrcpyW(pVTextA->Text + OrgALen, pVTextB->Text);
			pVTextA->Text[pVTextA->Length] = 0;
		}
	}

	return TRUE;
}





BOOL VTextInsertCharA(pVTEXTA pVText, char ch, int InsertBefore)
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
		pVText->Text = realloc(pVText->Text, pVText->Capibility * sizeof(char));
	}
	else
	{
		pVText->Text = malloc(pVText->Capibility * sizeof(char));
	}

	//内容后移

	memmove(pVText->Text + InsertBefore + 1, pVText->Text + InsertBefore, (pVText->Length - 1 - InsertBefore) * sizeof(char));
	pVText->Text[pVText->Length] = 0;
	pVText->Text[InsertBefore] = ch;

	return TRUE;
}

BOOL VTextInsertCharW(pVTEXTW pVText, WCHAR ch, int InsertBefore)
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
		pVText->Text = realloc(pVText->Text, pVText->Capibility * sizeof(WCHAR));
	}
	else
	{
		pVText->Text = malloc(pVText->Capibility * sizeof(WCHAR));
	}

	//内容后移

	memmove(pVText->Text + InsertBefore + 1, pVText->Text + InsertBefore, (pVText->Length - 1 - InsertBefore) * sizeof(WCHAR));
	pVText->Text[pVText->Length] = 0;
	pVText->Text[InsertBefore] = ch;

	return TRUE;
}


BOOL VTextDeleteCharA(pVTEXTA pVText, int DeleteBefore)
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

	memmove(pVText->Text + DeleteBefore - 1, pVText->Text + DeleteBefore, (pVText->Length - DeleteBefore) * sizeof(char));


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
		pVText->Text = realloc(pVText->Text, pVText->Capibility * sizeof(char));
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

BOOL VTextDeleteCharW(pVTEXTW pVText, int DeleteBefore)
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

	memmove(pVText->Text + DeleteBefore - 1, pVText->Text + DeleteBefore, (pVText->Length - DeleteBefore) * sizeof(WCHAR));


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
		pVText->Text = realloc(pVText->Text, pVText->Capibility * sizeof(WCHAR));
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


