#include<Windows.h>
#include"EasyWindow.h"

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
