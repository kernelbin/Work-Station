#pragma once
#include<Windows.h>
#include"Global.h"
//�й����õ�ͷ�ļ�

int TryLoadSettings();
int SaveSettings(BOOL bCreate);

//TryLoadSettings�ķ���ֵ����
#define SETTINGS_LOADED 0			//�ҵ����ò����سɹ�
#define SETTINGS_NOTFOUND 1			//û�ҵ�����
#define SETTINGS_ACCESS_DENIED 2	//ûȨ�޷���
#define SETTINGS_OLD 3				//�������ϰ汾��
#define SETTINGS_ERROR 4			//����δ֪����



