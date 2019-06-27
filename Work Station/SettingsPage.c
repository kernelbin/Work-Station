#include<Windows.h>
#include"EasyWindow.h"
#include"Global.h"
/*����ҳ���Ϊ��飺
1. ��������
2. ����̨����
3. �������
4.....

*/

typedef struct __tagSettingSection
{
	TCHAR SectionName[32];//��������
	//TODO: �½�һ���ṹ�壬����������ƣ�ֵ���Լ�����ص�����

	EZWND NameStatic;//�ı���
}SETTING_SECTION;


SETTING_SECTION SettingSection[] = {
{TEXT("����"),0},
{TEXT("����̨"),0}
};

#define SETTING_SEC_NUM (sizeof(SettingSection)/sizeof(SettingSection[0]))


EZWNDPROC SettingsPageProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	static EZWND SettingsTitle;
	switch (message)
	{
	case EZWM_CREATE:
		SettingsTitle = CreateEZStyleWindow(ezWnd, TEXT("����"), EZS_CHILD | EZS_STATIC, 0, 0, 0, 0);

		for (int i = 0; i < SETTING_SEC_NUM; i++)
		{
			SettingSection[i].NameStatic = CreateEZStyleWindow(ezWnd, SettingSection[i].SectionName,
				EZS_CHILD | EZS_STATIC, 0, 0, 0, 0);
		}
		return 0;
	case EZWM_USER_NOTIFY:
		
		return 1000;
	case EZWM_SIZE:
		//����Ԫ��
		MoveEZWindow(SettingsTitle, PAGE_PADDING + 30, PAGE_PADDING, ezWnd->Width - 2 * PAGE_PADDING - 30, 55, 0);
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
			MoveEZWindow(SettingSection[iSection].NameStatic, PAGE_PADDING + 38, CountY + PAGE_PADDING, ezWnd->Width - 2 * PAGE_PADDING - 40, 40, 0);

			FontForm.lfHeight = 40 * (5.0 / 7.0);

			EZSendMessage(SettingSection[iSection].NameStatic, EZWM_SETFONT, 0, &FontForm);
			EZSendMessage(SettingSection[iSection].NameStatic, EZWM_SETTEXTALIGN, DT_VCENTER | DT_SINGLELINE, 0);

			CountY += 50;

			//�������Section�ڵĿؼ�

			//�������

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
