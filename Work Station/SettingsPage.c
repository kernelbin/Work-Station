#include<Windows.h>
#include"EasyWindow.h"
#include"Global.h"

#define SETTING_TYPE_PATH 1

#define SETTING_NTF_EZMSG 1
typedef struct __tagSettingItem
{
	TCHAR ItemName[32];//����������
	int ItemType;//��������

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

	//���ûص�����

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

	//}link;//��ʼֵ

	EZWND NameStatic;
	EZWND ControlWnd[4];//�ؼ����Ҿ���4������
}SETTING_ITEM;

SETTING_ITEM UISettings[] = {
	{
		.ItemName = TEXT("����ͼƬ"),
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
	TCHAR SectionName[32];//��������
	//TODO: �½�һ���ṹ�壬����������ƣ�ֵ���Լ�����ص�����
	SETTING_ITEM* SettingItem;
	int ItemCount;
	EZWND NameStatic;//�ı���

}SETTING_SECTION;


SETTING_SECTION SettingSection[] = {
{TEXT("����"),UISettings,_countof(UISettings),0},
{TEXT("����̨"),0,0,0}
};

#define SETTING_SEC_NUM (sizeof(SettingSection)/sizeof(SettingSection[0]))


EZWNDPROC SettingsPageProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	static EZWND SettingsTitle;
	switch (message)
	{
	case EZWM_CREATE:
	{
		SettingsTitle = CreateEZStyleWindow(ezWnd, TEXT("����"), EZS_CHILD | EZS_STATIC, 0, 0, 0, 0);
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
				//���δ������������
				SettingSection[iSection].SettingItem[iItem].NameStatic =
					CreateEZStyleWindow(ezWnd, SettingSection[iSection].SettingItem[iItem].ItemName, EZS_CHILD | EZS_STATIC, 0, 0, 0, 0);
				FontForm.lfHeight = 34 * (5.0 / 7.0);
				EZSendMessage(SettingSection[iSection].SettingItem[iItem].NameStatic, EZWM_SETFONT, 0, &FontForm);
				EZSendMessage(SettingSection[iSection].SettingItem[iItem].NameStatic, EZWM_SETTEXTALIGN, DT_VCENTER | DT_SINGLELINE, 0);

				switch (SettingSection[iSection].SettingItem[iItem].ItemType)
				{
				case SETTING_TYPE_PATH:
					//����һ��·���ı��򣨾�̬����һ�������ť
					//���س�ʼ��
					//lstrcpy(SettingSection[iSection].SettingItem[iItem].v.valStr, SettingSection[iSection].SettingItem[iItem].link.valStr);

					SettingSection[iSection].SettingItem[iItem].ControlWnd[0] =
						CreateEZStyleWindow(ezWnd, SettingSection[iSection].SettingItem[iItem].u.PathEdit.Path, EZS_CHILD | EZS_STATIC, 0, 0, 0, 0);
					EZSendMessage(SettingSection[iSection].SettingItem[iItem].ControlWnd[0],
						EZWM_SETFONT, 0, &FontForm);
					EZSendMessage(SettingSection[iSection].SettingItem[iItem].ControlWnd[0],
						EZWM_SETTEXTALIGN, DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_PATH_ELLIPSIS, 0);

					SettingSection[iSection].SettingItem[iItem].ControlWnd[1] = CreateEZWindow(ezWnd, 0, 0, 0, 0, ColorBlockProc);

					SettingSection[iSection].SettingItem[iItem].ControlWnd[2] =
						CreateEZStyleWindow(ezWnd, TEXT("���"), EZS_CHILD | EZS_BUTTON, 0, 0, 0, 0);
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
		//����Ԫ��
	{
		MoveEZWindow(SettingsTitle, PAGE_PADDING + 20, PAGE_PADDING, ezWnd->Width - 2 * PAGE_PADDING - 20, 55, 0);

		//�����￪ʼԪ�ز���
		int CountX;//��ǰX���꣨����Padding��
		int CountY;//��ǰ��Y���꣨����Padding��

		CountY = 70;
		for (int iSection = 0; iSection < SETTING_SEC_NUM; iSection++)
		{
			//���Ȱ��ű���
			MoveEZWindow(SettingSection[iSection].NameStatic, PAGE_PADDING + 30, CountY + PAGE_PADDING, ezWnd->Width - 2 * PAGE_PADDING - 30, 40, 0);

			CountY += 45;

			//�������Section�ڵĿؼ�
			for (int iItem = 0; iItem < SettingSection[iSection].ItemCount; iItem++)
			{
				//���ű����λ�ã��������ŵ��¡��Ų���Ҳ����ȥ
				MoveEZWindow(SettingSection[iSection].SettingItem[iItem].NameStatic,
					PAGE_PADDING + 40, PAGE_PADDING + CountY, ezWnd->Width - 2 * PAGE_PADDING - 40, 34, 0);


				CountX = 140;

				switch (SettingSection[iSection].SettingItem[iItem].ItemType)
				{
				case SETTING_TYPE_PATH:
					//�����ȷ��λ�����Ӵ��ڡ���һ����̬·���ı���һ����ť��
					//�������빻����������
					if (CountX + PATHCONTROL_MINLEN + PATHCONTROL_BTN_LEN + CONTROL_GAP * 2 <= ezWnd->Width - PAGE_PADDING * 2)
					{

						//�����ŵ��£�����һ�¾�̬·���ı��򳤶�
						MoveEZWindow(SettingSection[iSection].SettingItem[iItem].ControlWnd[0],
							CountX + PAGE_PADDING, CountY + PAGE_PADDING, min(PATHCONTROL_MAXLEN, ezWnd->Width - PAGE_PADDING * 2 - (CountX + PATHCONTROL_BTN_LEN + CONTROL_GAP * 2)), 34, 0);

						//�»���
						MoveEZWindow(SettingSection[iSection].SettingItem[iItem].ControlWnd[1],
							CountX + PAGE_PADDING, CountY + PAGE_PADDING + 34, min(PATHCONTROL_MAXLEN, ezWnd->Width - PAGE_PADDING * 2 - (CountX + PATHCONTROL_BTN_LEN + CONTROL_GAP * 2)), 2, 0);

						MoveEZWindow(SettingSection[iSection].SettingItem[iItem].ControlWnd[2],
							CountX + PAGE_PADDING +
							min(PATHCONTROL_MAXLEN, ezWnd->Width - PAGE_PADDING * 2 - (CountX + PATHCONTROL_BTN_LEN + CONTROL_GAP * 2)) + CONTROL_GAP, CountY + PAGE_PADDING, PATHCONTROL_BTN_LEN, 34, 0);


						//CreateEZStyleWindow(ezWnd, TEXT("�����"), EZS_CHILD | EZS_BUTTON, 0, 0, 0, 0);
					}

					else
					{
						CountX = 40;
						CountY += 34;
						//�ж��ܲ�����������
						if (CountX + PATHCONTROL_MINLEN + PATHCONTROL_BTN_LEN + CONTROL_GAP * 2 <= ezWnd->Width - PAGE_PADDING * 2)
						{
							//�����ŵ��£�����һ�¾�̬·���ı��򳤶�
							MoveEZWindow(SettingSection[iSection].SettingItem[iItem].ControlWnd[0],
								CountX + PAGE_PADDING, CountY + PAGE_PADDING, min(PATHCONTROL_MAXLEN, ezWnd->Width - PAGE_PADDING * 2 - (CountX + PATHCONTROL_BTN_LEN + CONTROL_GAP * 2)), 34, 0);
							//�»���
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
							CountY += 40;//�صض��һ�㣬��ֹ�ؼ��غ�
							MoveEZWindow(SettingSection[iSection].SettingItem[iItem].ControlWnd[2],
								CountX + PAGE_PADDING, CountY + PAGE_PADDING, PATHCONTROL_BTN_LEN, 34, 0);
						}

					}

					break;
				}

				CountY += 20;
			}


			//�������
			CountY += 50;
		}
	}
		
		break;
	case EZWM_COMMAND:
		//�õ�Section��Item
	{
		int iSection = (((EZWND)lParam)->ezID & 0b11110000) >> 4;
		int iItem = (((EZWND)lParam)->ezID & 0b00001111);

		//�����ͷ���
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

		//������ѻص�����
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