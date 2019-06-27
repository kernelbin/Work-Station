#include<Windows.h>
#include"Global.h"

int InitGDIObject()
{
	AppColor = RGB(33, 150, 243);
	BackgroundColor = RGB(225, 226, 225);
	LightBackgroundColor = RGB(245, 245, 246);

	lstrcpy(FontFaceName, TEXT("Î¢ÈíÑÅºÚ"));

	FontForm.lfCharSet = DEFAULT_CHARSET;
	FontForm.lfClipPrecision = 0;
	FontForm.lfEscapement = 0;
	lstrcpy(FontForm.lfFaceName, FontFaceName);
	FontForm.lfHeight = 0;
	FontForm.lfItalic = 0;
	FontForm.lfOrientation = 0;
	FontForm.lfOutPrecision = 0;
	FontForm.lfPitchAndFamily = 0;
	FontForm.lfQuality = 0;
	FontForm.lfStrikeOut = 0;
	FontForm.lfUnderline = 0;
	FontForm.lfWeight = 400;
	FontForm.lfWidth = 0;

	return 0;
}

HFONT CreateAppFont(int Height, int Weight)
{
	return CreateFont(Height, 0, 0, 0, Weight, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, FontFaceName);
}