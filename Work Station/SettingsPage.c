#include<Windows.h>
#include"EasyWindow.h"
#include"Global.h"
/*设置页面分为板块：
1. 界面设置
2. 控制台设置
3. 插件设置
4.....

*/

typedef struct __tagSettingSection
{
	TCHAR SectionName[32];//段落名称
	//TODO: 新建一个结构体，存放设置名称，值，以及变更回调函数

	EZWND NameStatic;//文本框
}SETTING_SECTION;


SETTING_SECTION SettingSection[] = {
{TEXT("界面"),0},
{TEXT("控制台"),0}
};

#define SETTING_SEC_NUM (sizeof(SettingSection)/sizeof(SettingSection[0]))


EZWNDPROC SettingsPageProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	static EZWND SettingsTitle;
	switch (message)
	{
	case EZWM_CREATE:
		SettingsTitle = CreateEZStyleWindow(ezWnd, TEXT("设置"), EZS_CHILD | EZS_STATIC, 0, 0, 0, 0);

		for (int i = 0; i < SETTING_SEC_NUM; i++)
		{
			SettingSection[i].NameStatic = CreateEZStyleWindow(ezWnd, SettingSection[i].SectionName,
				EZS_CHILD | EZS_STATIC, 0, 0, 0, 0);
		}
		return 0;
	case EZWM_USER_NOTIFY:
		
		return 1000;
	case EZWM_SIZE:
		//布局元素
		MoveEZWindow(SettingsTitle, PAGE_PADDING + 30, PAGE_PADDING, ezWnd->Width - 2 * PAGE_PADDING - 30, 55, 0);
		FontForm.lfHeight = 55 * (5.0 / 7.0);
		EZSendMessage(SettingsTitle, EZWM_SETFONT, 0, &FontForm);
		EZSendMessage(SettingsTitle, EZWM_SETTEXTALIGN, DT_VCENTER|DT_SINGLELINE, 0);
		//从这里开始元素布局
		int CountX;//当前X坐标（不计Padding）
		int CountY;//当前行Y坐标（不计Padding）

		CountY = 70;
		for (int iSection = 0; iSection < SETTING_SEC_NUM; iSection++)
		{
			//首先安排标题
			MoveEZWindow(SettingSection[iSection].NameStatic, PAGE_PADDING + 38, CountY + PAGE_PADDING, ezWnd->Width - 2 * PAGE_PADDING - 40, 40, 0);

			FontForm.lfHeight = 40 * (5.0 / 7.0);

			EZSendMessage(SettingSection[iSection].NameStatic, EZWM_SETFONT, 0, &FontForm);
			EZSendMessage(SettingSection[iSection].NameStatic, EZWM_SETTEXTALIGN, DT_VCENTER | DT_SINGLELINE, 0);

			CountY += 50;

			//这里，安排Section内的控件

			//这里，留空

		}

		break;
	case EZWM_DRAW:
		PatBlt(wParam, PAGE_PADDING, PAGE_PADDING, ezWnd->Width - PAGE_PADDING*2, ezWnd->Height - PAGE_PADDING*2, WHITENESS);
		HBRUSH hOldBrush = SelectObject(wParam,CreateSolidBrush(AppColor));
		PatBlt(wParam, PAGE_PADDING, PAGE_PADDING + 60, ezWnd->Width - PAGE_PADDING * 2, 6, PATCOPY);
		DeleteObject(SelectObject(wParam, hOldBrush));
		return 0;
	}
	return 0;
}
