#include<Windows.h>
#include"Global.h"
#include"libtcc.h"


void* WINAPI ConsoleThread()
{
	//printf("Microsoft Windows [版本 10.0.18362.175]\n(c)2019 Microsoft Corporation。保留所有权利。\n\nC:\\Users\\11603>");


	while (1)
	{
		//编译模式，等待事件
		DWORD bWaitResult = WaitForSingleObject(hEventStartCompile, INFINITE);
		switch (bWaitResult)
		{
		case WAIT_OBJECT_0:
			//等待成功
			break;
		case WAIT_ABANDONED:
			ErrorMsgBox(TEXT("WaitForSingleObject返回WAIT_ABANDONED"));
			ExitProcess(0);
			break;

		default:
			ErrorMsgBox(TEXT("WaitForSingleObject等待异常"));
			ExitProcess(0);
			break;
			/*	DONE  这里！第一件事情是把这个switch补全，处理妥当
				第二件事是在ConsolePage里添加判定现在是运行模式还是等待编译模式的代码
				并且在等待编译模式的时候不把数据写入管道而是申请内存存起来，激活事件。
				第三件事情是，这个switch处理完后编译代码，然后把内存释放
				程序结束运行的时候，发送一条消息给ConsolePage(注意是send不是post)指示切换状态*/
		}

		//此处编译 & 运行代码
		//这里！！！把这个函数移到另一个文件？ 我预计这个函数体积将会很庞大
			//两个lib已经创建好了，头文件（只有libtcc.h）也移进来了。
			//在新文件里引用dll并且包含头文件，试着编译



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


		printf("\n***** 开始编译 ******\n");
		if (tcc_compile_string(s, CodeBuffer) == -1)

		{
			//system("pause");

			printf("编译出错。\n");
			//通知

			return 1;

		}
		printf("\n***** 编译结束 ******\n\n");
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