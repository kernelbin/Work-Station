#include<Windows.h>
#include"EasyWindow.h"

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

}SETTING_SECTION;


EZWNDPROC SettingsPageProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case EZWM_CREATE:
		return 0;
	}
	return 0;
}
