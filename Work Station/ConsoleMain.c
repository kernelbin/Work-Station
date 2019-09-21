#include<Windows.h>
#include"Global.h"
#include"libtcc.h"


void* WINAPI ConsoleThread()
{
	//printf("Microsoft Windows [�汾 10.0.18362.175]\n(c)2019 Microsoft Corporation����������Ȩ����\n\nC:\\Users\\11603>");


	while (1)
	{
		//����ģʽ���ȴ��¼�
		DWORD bWaitResult = WaitForSingleObject(hEventStartCompile, INFINITE);
		switch (bWaitResult)
		{
		case WAIT_OBJECT_0:
			//�ȴ��ɹ�
			break;
		case WAIT_ABANDONED:
			ErrorMsgBox(TEXT("WaitForSingleObject����WAIT_ABANDONED"));
			ExitProcess(0);
			break;

		default:
			ErrorMsgBox(TEXT("WaitForSingleObject�ȴ��쳣"));
			ExitProcess(0);
			break;
			/*	DONE  �����һ�������ǰ����switch��ȫ�������׵�
				�ڶ���������ConsolePage������ж�����������ģʽ���ǵȴ�����ģʽ�Ĵ���
				�����ڵȴ�����ģʽ��ʱ�򲻰�����д��ܵ����������ڴ�������������¼���
				�����������ǣ����switch������������룬Ȼ����ڴ��ͷ�
				����������е�ʱ�򣬷���һ����Ϣ��ConsolePage(ע����send����post)ָʾ�л�״̬*/
		}

		//�˴����� & ���д���
		//�����������������Ƶ���һ���ļ��� ��Ԥ������������������Ӵ�
			//����lib�Ѿ��������ˣ�ͷ�ļ���ֻ��libtcc.h��Ҳ�ƽ����ˡ�
			//�����ļ�������dll���Ұ���ͷ�ļ������ű���



		//char my_program[] = "int myadd(int a, int b){return a+b;}";

		TCCState* s = tcc_new();
		tcc_set_lib_path(s, "C:\\Users\\11603\\Desktop\\TCC\\tinycc");

		tcc_add_sysinclude_path(s, "C:\\Users\\11603\\Desktop\\TCC\\tinycc\\include");
		tcc_add_sysinclude_path(s, "C:\\Users\\11603\\Desktop\\TCC\\tinycc\\include\\winapi");
		tcc_add_sysinclude_path(s, "C:\\Users\\11603\\Desktop\\TCC\\tinycc\\include\\sys");

		tcc_add_library_path(s, "C:\\Users\\11603\\Desktop\\TCC\\tinycc\\lib");
		tcc_add_library_path(s, "C:\\Users\\11603\\Desktop\\TCC\\tinycc\\win32\\lib");

		int a;

		a = tcc_add_library(s, "user32");
		a = tcc_add_library(s, "gdi32");
		a = tcc_add_library(s, "kernel32");
		//a = tcc_add_library(s, "msvcrt");

		tcc_set_output_type(s, TCC_OUTPUT_MEMORY);


		printf("\n***** ��ʼ���� ******\n");
		if (tcc_compile_string(s, CodeBuffer) == -1)

		{
			//system("pause");

			printf("�������\n");
			//֪ͨ

			return 1;

		}
		printf("\n***** ������� ******\n\n");
		//system("pause");


#pragma  warning( push ) 
#pragma  warning( disable: 4996 )

		//tcc_add_symbol(s, "printf", printf);
		//tcc_add_symbol(s, "scanf", scanf);

#pragma  warning(  pop  ) 

		
		

		int size = tcc_relocate(s, NULL);

		if (size == -1)

			return 1;

		//system("pause");


		void* mem = malloc(size);

		tcc_relocate(s, mem);

		int (*CommandMain)();

		CommandMain = (int(*)())tcc_get_symbol(s, "main");

		


		if (!CommandMain) return 1;

		CommandMain();

		
		tcc_delete(s);

		free(mem);

		free(CodeBuffer);

	}
	return 0;
}