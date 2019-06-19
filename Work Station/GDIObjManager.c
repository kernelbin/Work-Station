#include<Windows.h>
#include"Global.h"

int InitGDIObject()
{
	AppColor = RGB(33, 150, 243);
	BackgroundColor = RGB(225, 226, 225);
	LightBackgroundColor = RGB(245, 245, 246);

	lstrcpy(FontFaceName, TEXT("Î¢ÈíÑÅºÚ"));
}

HFONT CreateAppFont(int Height, int Weight)
{
	return CreateFont(Height, 0, 0, 0, Weight, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, FontFaceName);
}