#include<Windows.h>
#include"EasyWindow.h"
#include"Global.h"

#define SETTING_TYPE_PATH 1

#define SETTING_NTF_EZMSG 1
typedef struct __tagSettingItem
{
	TCHAR ItemName[32];//设置项名称
	int ItemType;//设置类型

	union
	{
		struct
		{
			TCHAR *Path;
			TCHAR* Name;

			TCHAR* Filter;
			//TCHAR Related;
		}PathEdit;


	}u;

	//设置回调函数

	int NotificationType;

	union
	{
		struct
		{
			EZWND * pezWnd;
			int message;
		}ezmsg;
	}n;
	//union
	//{
	//	int valInt;
	//	TCHAR valStr[MAX_PATH];

	//}v;

	//union
	//{
	//	int valInt;
	//	TCHAR* valStr;

	//}link;//初始值

	EZWND NameStatic;
	EZWND ControlWnd[4];//控件，我觉得4个够了
}SETTING_ITEM;

SETTING_ITEM UISettings[] = {
	{
		.ItemName = TEXT("背景图片"),
		.ItemType = SETTING_TYPE_PATH,
		.u.PathEdit.Path = BkgndPicPath,
		.u.PathEdit.Filter = TEXT("JPEG (*.jpg)\0*.jpg\0All Files(*.*)\0*.*\0\0"),

		.NotificationType = SETTING_NTF_EZMSG,
		.n.ezmsg.pezWnd = &MainWnd,
		.n.ezmsg.message = EZWM_UPDATE_SETTINGS
	}
};



typedef struct __tagSettingSection
{
	TCHAR SectionName[32];//段落名称
	//TODO: 新建一个结构体，存放设置名称，值，以及变更回调函数
	SETTING_ITEM* SettingItem;
	int ItemCount;
	EZWND NameStatic;//文本框

}SETTING_SECTION;


SETTING_SECTION SettingSection[] = {
{TEXT("界面"),UISettings,_countof(UISettings),0},
{TEXT("控制台"),0,0,0}
};

#define SETTING_SEC_NUM (sizeof(SettingSection)/sizeof(SettingSection[0]))


EZWNDPROC SettingsPageProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	static EZWND SettingsTitle;
	switch (message)
	{
	case EZWM_CREATE:
	{
		SettingsTitle = CreateEZStyleWindow(ezWnd, TEXT("设置"), EZS_CHILD | EZS_STATIC, 0, 0, 0, 0);
		FontForm.lfHeight = 55 * (5.0 / 7.0);
		EZSendMessage(SettingsTitle, EZWM_SETFONT, 0, &FontForm);
		EZSendMessage(SettingsTitle, EZWM_SETTEXTALIGN, DT_VCENTER | DT_SINGLELINE, 0);


		for (int iSection = 0; iSection < SETTING_SEC_NUM; iSection++)
		{
			SettingSection[iSection].NameStatic = CreateEZStyleWindow(ezWnd, SettingSection[iSection].SectionName,
				EZS_CHILD | EZS_STATIC, 0, 0, 0, 0);

			FontForm.lfHeight = 40 * (5.0 / 7.0);
			EZSendMessage(SettingSection[iSection].NameStatic, EZWM_SETFONT, 0, &FontForm);
			EZSendMessage(SettingSection[iSection].NameStatic, EZWM_SETTEXTALIGN, DT_VCENTER | DT_SINGLELINE, 0);


			for (int iItem = 0; iItem < SettingSection[iSection].ItemCount; iItem++)
			{
				//依次创建标题和主体
				SettingSection[iSection].SettingItem[iItem].NameStatic =
					CreateEZStyleWindow(ezWnd, SettingSection[iSection].SettingItem[iItem].ItemName, EZS_CHILD | EZS_STATIC, 0, 0, 0, 0);
				FontForm.lfHeight = 34 * (5.0 / 7.0);
				EZSendMessage(SettingSection[iSection].SettingItem[iItem].NameStatic, EZWM_SETFONT, 0, &FontForm);
				EZSendMessage(SettingSection[iSection].SettingItem[iItem].NameStatic, EZWM_SETTEXTALIGN, DT_VCENTER | DT_SINGLELINE, 0);

				switch (SettingSection[iSection].SettingItem[iItem].ItemType)
				{
				case SETTING_TYPE_PATH:
					//创建一个路径文本框（静态）和一个浏览按钮
					//加载初始项
					//lstrcpy(SettingSection[iSection].SettingItem[iItem].v.valStr, SettingSection[iSection].SettingItem[iItem].link.valStr);

					SettingSection[iSection].SettingItem[iItem].ControlWnd[0] =
						CreateEZStyleWindow(ezWnd, SettingSection[iSection].SettingItem[iItem].u.PathEdit.Path, EZS_CHILD | EZS_STATIC, 0, 0, 0, 0);
					EZSendMessage(SettingSection[iSection].SettingItem[iItem].ControlWnd[0],
						EZWM_SETFONT, 0, &FontForm);
					EZSendMessage(SettingSection[iSection].SettingItem[iItem].ControlWnd[0],
						EZWM_SETTEXTALIGN, DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_PATH_ELLIPSIS, 0);

					SettingSection[iSection].SettingItem[iItem].ControlWnd[1] = CreateEZWindow(ezWnd, 0, 0, 0, 0, ColorBlockProc);

					SettingSection[iSection].SettingItem[iItem].ControlWnd[2] =
						CreateEZStyleWindow(ezWnd, TEXT("浏览"), EZS_CHILD | EZS_BUTTON, 0, 0, 0, 0);
					EZSendMessage(SettingSection[iSection].SettingItem[iItem].ControlWnd[2],
						EZWM_SETFONT, 0, &FontForm);
					EZSendMessage(SettingSection[iSection].SettingItem[iItem].ControlWnd[2],
						EZWM_SETTEXTALIGN, DT_CENTER | DT_VCENTER | DT_SINGLELINE, 0);
					EZSendMessage(SettingSection[iSection].SettingItem[iItem].ControlWnd[2],
						EZWM_SETCOLOR, RGB(128, 128, 128), RGB(20, 20, 20));

					SettingSection[iSection].SettingItem[iItem].ControlWnd[2]->ezID = (iSection << 4) + iItem;
					break;

				}
			}
		}
		return 0;
	}
		
	case EZWM_USER_NOTIFY:

		return 1000;
	case EZWM_SIZE:
		//布局元素
	{
		MoveEZWindow(SettingsTitle, PAGE_PADDING + 20, PAGE_PADDING, ezWnd->Width - 2 * PAGE_PADDING - 20, 55, 0);

		//从这里开始元素布局
		int CountX;//当前X坐标（不计Padding）
		int CountY;//当前行Y坐标（不计Padding）

		CountY = 70;
		for (int iSection = 0; iSection < SETTING_SEC_NUM; iSection++)
		{
			//首先安排标题
			MoveEZWindow(SettingSection[iSection].NameStatic, PAGE_PADDING + 30, CountY + PAGE_PADDING, ezWnd->Width - 2 * PAGE_PADDING - 30, 40, 0);

			CountY += 45;

			//这里，安排Section内的控件
			for (int iItem = 0; iItem < SettingSection[iSection].ItemCount; iItem++)
			{
				//安排标题的位置，这个必须放的下。放不下也塞进去
				MoveEZWindow(SettingSection[iSection].SettingItem[iItem].NameStatic,
					PAGE_PADDING + 40, PAGE_PADDING + CountY, ezWnd->Width - 2 * PAGE_PADDING - 40, 34, 0);


				CountX = 140;

				switch (SettingSection[iSection].SettingItem[iItem].ItemType)
				{
				case SETTING_TYPE_PATH:
					//这里！正确定位两个子窗口。（一个静态路径文本框，一个按钮）
					//测量距离够不够放两个
					if (CountX + PATHCONTROL_MINLEN + PATHCONTROL_BTN_LEN + CONTROL_GAP * 2 <= ezWnd->Width - PAGE_PADDING * 2)
					{

						//俩都放得下，计算一下静态路径文本框长度
						MoveEZWindow(SettingSection[iSection].SettingItem[iItem].ControlWnd[0],
							CountX + PAGE_PADDING, CountY + PAGE_PADDING, min(PATHCONTROL_MAXLEN, ezWnd->Width - PAGE_PADDING * 2 - (CountX + PATHCONTROL_BTN_LEN + CONTROL_GAP * 2)), 34, 0);

						//下划线
						MoveEZWindow(SettingSection[iSection].SettingItem[iItem].ControlWnd[1],
							CountX + PAGE_PADDING, CountY + PAGE_PADDING + 34, min(PATHCONTROL_MAXLEN, ezWnd->Width - PAGE_PADDING * 2 - (CountX + PATHCONTROL_BTN_LEN + CONTROL_GAP * 2)), 2, 0);

						MoveEZWindow(SettingSection[iSection].SettingItem[iItem].ControlWnd[2],
							CountX + PAGE_PADDING +
							min(PATHCONTROL_MAXLEN, ezWnd->Width - PAGE_PADDING * 2 - (CountX + PATHCONTROL_BTN_LEN + CONTROL_GAP * 2)) + CONTROL_GAP, CountY + PAGE_PADDING, PATHCONTROL_BTN_LEN, 34, 0);


						//CreateEZStyleWindow(ezWnd, TEXT("留浏览"), EZS_CHILD | EZS_BUTTON, 0, 0, 0, 0);
					}

					else
					{
						CountX = 40;
						CountY += 34;
						//判断能不能塞下两个
						if (CountX + PATHCONTROL_MINLEN + PATHCONTROL_BTN_LEN + CONTROL_GAP * 2 <= ezWnd->Width - PAGE_PADDING * 2)
						{
							//俩都放得下，计算一下静态路径文本框长度
							MoveEZWindow(SettingSection[iSection].SettingItem[iItem].ControlWnd[0],
								CountX + PAGE_PADDING, CountY + PAGE_PADDING, min(PATHCONTROL_MAXLEN, ezWnd->Width - PAGE_PADDING * 2 - (CountX + PATHCONTROL_BTN_LEN + CONTROL_GAP * 2)), 34, 0);
							//下划线
							MoveEZWindow(SettingSection[iSection].SettingItem[iItem].ControlWnd[1],
								CountX + PAGE_PADDING, CountY + PAGE_PADDING + 34, min(PATHCONTROL_MAXLEN, ezWnd->Width - PAGE_PADDING * 2 - (CountX + PATHCONTROL_BTN_LEN + CONTROL_GAP * 2)), 2, 0);

							MoveEZWindow(SettingSection[iSection].SettingItem[iItem].ControlWnd[2],
								CountX + PAGE_PADDING +
								min(PATHCONTROL_MAXLEN, ezWnd->Width - PAGE_PADDING * 2 - (CountX + PATHCONTROL_BTN_LEN + CONTROL_GAP * 2)) + CONTROL_GAP, CountY + PAGE_PADDING, PATHCONTROL_BTN_LEN, 34, 0);
						}
						else
						{
							MoveEZWindow(SettingSection[iSection].SettingItem[iItem].ControlWnd[0],
								CountX + PAGE_PADDING, CountY + PAGE_PADDING, min(PATHCONTROL_MAXLEN, ezWnd->Width - PAGE_PADDING * 2 - (CountX + CONTROL_GAP)), 34, 0);

							MoveEZWindow(SettingSection[iSection].SettingItem[iItem].ControlWnd[1],
								CountX + PAGE_PADDING, CountY + PAGE_PADDING + 34, min(PATHCONTROL_MAXLEN, ezWnd->Width - PAGE_PADDING * 2 - (CountX + CONTROL_GAP)), 2, 0);

							CountX = 40;
							CountY += 40;//特地多加一点，防止控件重合
							MoveEZWindow(SettingSection[iSection].SettingItem[iItem].ControlWnd[2],
								CountX + PAGE_PADDING, CountY + PAGE_PADDING, PATHCONTROL_BTN_LEN, 34, 0);
						}

					}

					break;
				}

				CountY += 20;
			}


			//这里，留空
			CountY += 50;
		}
	}
		
		break;
	case EZWM_COMMAND:
		//得到Section和Item
	{
		int iSection = (((EZWND)lParam)->ezID & 0b11110000) >> 4;
		int iItem = (((EZWND)lParam)->ezID & 0b00001111);

		//按类型分类
		switch (SettingSection[iSection].SettingItem[iItem].ItemType)
		{
		case SETTING_TYPE_PATH:
		{
			OpenFileDialog(NULL, SettingSection[iSection].SettingItem[iItem].u.PathEdit.Filter,
				SettingSection[iSection].SettingItem[iItem].u.PathEdit.Path,
				SettingSection[iSection].SettingItem[iItem].u.PathEdit.Name);
			//MessageBox(NULL, BkgndPicPath, FileTitleResult, 0);

			EZSendMessage(SettingSection[iSection].SettingItem[iItem].ControlWnd[0],
				EZWM_SETTEXT,
				SettingSection[iSection].SettingItem[iItem].u.PathEdit.Path,
				0);
			break;
		}
			
		}

		//变更提醒回调函数
		switch (SettingSection[iSection].SettingItem[iItem].NotificationType)
		{
		case SETTING_NTF_EZMSG:
			EZSendMessage(*(SettingSection[iSection].SettingItem[iItem].n.ezmsg.pezWnd),
				SettingSection[iSection].SettingItem[iItem].n.ezmsg.message, 0, 0);
			break;
		}
		
	}
		
		break;
	case EZWM_DRAW:
		PatBlt(wParam, PAGE_PADDING, PAGE_PADDING, ezWnd->Width - PAGE_PADDING * 2, ezWnd->Height - PAGE_PADDING * 2, WHITENESS);
		HBRUSH hOldBrush = SelectObject(wParam, CreateSolidBrush(AppColor));
		PatBlt(wParam, PAGE_PADDING, PAGE_PADDING + 60, ezWnd->Width - PAGE_PADDING * 2, 6, PATCOPY);
		DeleteObject(SelectObject(wParam, hOldBrush));
		return 0;
	}
	return 0;
}


EZWNDPROC ColorBlockProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case EZWM_DRAW:
	{
		HBRUSH OldBrush = SelectObject(wParam, CreateSolidBrush(AppColor));
		PatBlt(wParam, 0, 0, ezWnd->Width, ezWnd->Height, PATCOPY);
		DeleteObject(SelectObject(wParam, OldBrush));
		break;
	}
	}

	return 0;
}