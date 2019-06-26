#include<Windows.h>
#include"EasyWindow.h"


EZWNDPROC SettingsPageProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case EZWM_CREATE:
		return 0;
	}
	return 0;
}
