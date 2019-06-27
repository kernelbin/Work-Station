#include<Windows.h>
#include<GdiPlus.h>
#pragma comment(lib, "Gdiplus.lib")  
using namespace Gdiplus;

GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR           gdiplusToken;


Image* pImage = NULL;


extern "C" BOOL InitGdiPlus()
{
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	return TRUE;
}



extern "C" BOOL CleanGdiPlus()
{
	delete pImage;
	GdiplusShutdown(gdiplusToken);
	return TRUE;
}




extern "C" HBITMAP LoadPicFromFile(TCHAR Path[])
{
	HBITMAP hBitmap = 0;
	Image* pImg;
	pImg = Bitmap::FromFile(Path, 0);
	((Bitmap*)pImg)->GetHBITMAP(RGB(0, 0, 0), &hBitmap);
	delete pImg;
	return hBitmap;
}
