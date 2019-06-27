#include<Windows.h>
#include"EasyWindow.h"
#include"Global.h"
/*����ҳ���Ϊ��飺
1. ��������
2. ����̨����
3. �������
4.....

*/

#define SETTING_TYPE_PATH 1


typedef struct __tagSettingItem
{
	TCHAR ItemName[32];//����������
	int ItemType;//��������
	union
	{
		int valInt;
		TCHAR valStr[MAX_PATH];

	}v;
	EZWND NameStatic;
}SETTING_ITEM;

SETTING_ITEM UISettings[] = {
	{
		.ItemName = TEXT("����ͼƬ"),
		.ItemType = SETTING_TYPE_PATH,
		.v.valStr = TEXT("")
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
		SettingsTitle = CreateEZStyleWindow(ezWnd, TEXT("����"), EZS_CHILD | EZS_STATIC, 0, 0, 0, 0);

		for (int iSection = 0; iSection < SETTING_SEC_NUM; iSection++)
		{
			SettingSection[iSection].NameStatic = CreateEZStyleWindow(ezWnd, SettingSection[iSection].SectionName,
				EZS_CHILD | EZS_STATIC, 0, 0, 0, 0);
			for (int iItem = 0; iItem < SettingSection[iSection].ItemCount; iItem++)
			{
				//���δ������������
				SettingSection[iSection].SettingItem[iItem].NameStatic =
					CreateEZStyleWindow(ezWnd, SettingSection[iSection].SettingItem[iItem].ItemName, EZS_CHILD | EZS_STATIC, 0, 0, 0, 0);
				//TODO: ������switchһ�£���������
			}
		}
		return 0;
	case EZWM_USER_NOTIFY:
		
		return 1000;
	case EZWM_SIZE:
		//����Ԫ��
		MoveEZWindow(SettingsTitle, PAGE_PADDING + 20, PAGE_PADDING, ezWnd->Width - 2 * PAGE_PADDING - 20, 55, 0);
		FontForm.lfHeight = 55 * (5.0 / 7.0);
		EZSendMessage(SettingsTitle, EZWM_SETFONT, 0, &FontForm);
		EZSendMessage(SettingsTitle, EZWM_SETTEXTALIGN, DT_VCENTER|DT_SINGLELINE, 0);
		//�����￪ʼԪ�ز���
		int CountX;//��ǰX���꣨����Padding��
		int CountY;//��ǰ��Y���꣨����Padding��

		CountY = 70;
		for (int iSection = 0; iSection < SETTING_SEC_NUM; iSection++)
		{
			//���Ȱ��ű���
			MoveEZWindow(SettingSection[iSection].NameStatic, PAGE_PADDING + 30, CountY + PAGE_PADDING, ezWnd->Width - 2 * PAGE_PADDING - 30, 40, 0);

			FontForm.lfHeight = 40 * (5.0 / 7.0);

			EZSendMessage(SettingSection[iSection].NameStatic, EZWM_SETFONT, 0, &FontForm);
			EZSendMessage(SettingSection[iSection].NameStatic, EZWM_SETTEXTALIGN, DT_VCENTER | DT_SINGLELINE, 0);

			CountY += 45;

			
			//�������Section�ڵĿؼ�
			for (int iItem = 0; iItem < SettingSection[iSection].ItemCount; iItem++)
			{
				//���ű����λ��
				MoveEZWindow(SettingSection[iSection].SettingItem[iItem].NameStatic,
					PAGE_PADDING + 40, PAGE_PADDING + CountY, ezWnd->Width - 2 * PAGE_PADDING - 40, 34, 0);
					
				FontForm.lfHeight = 34 * (5.0 / 7.0);
				EZSendMessage(SettingSection[iSection].SettingItem[iItem].NameStatic, EZWM_SETFONT, 0, &FontForm);
				EZSendMessage(SettingSection[iSection].SettingItem[iItem].NameStatic, EZWM_SETTEXTALIGN, DT_VCENTER | DT_SINGLELINE, 0);
				CountY += 34;
			}


			//�������
			CountY += 50;
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
