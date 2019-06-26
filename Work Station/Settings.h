#pragma once
#include<Windows.h>
#include"Global.h"
//有关设置的头文件

int TryLoadSettings();
int SaveSettings(BOOL bCreate);

//TryLoadSettings的返回值定义
#define SETTINGS_LOADED 0			//找到设置并加载成功
#define SETTINGS_NOTFOUND 1			//没找到设置
#define SETTINGS_ACCESS_DENIED 2	//没权限访问
#define SETTINGS_OLD 3				//设置是老版本的
#define SETTINGS_ERROR 4			//出现未知错误



