#pragma once
#include<Windows.h>
#include"EasyWindow.h"


//窗口过程声明

EZWNDPROC MainParentProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
EZWNDPROC MainWndProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);

EZWNDPROC LeftBorderProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
EZWNDPROC FixedBtnProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);

EZWNDPROC PageHolderProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);

EZWNDPROC MainPageProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);

EZWNDPROC NewScrollBarProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);

EZWNDPROC MainPageContentScrollProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);
EZWNDPROC MainPageContentProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);

EZWNDPROC PluginListProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);

EZWNDPROC ConsolePageProc(EZWND ezWnd, int message, WPARAM wParam, LPARAM lParam);