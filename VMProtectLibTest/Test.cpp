
#include <Windows.h>
#include <stdio.h>

#include "../VMProtectLib.h"

void TestMutation()
{
	VMProtect::BeginMutation("seq1");
	printf("code in seq1\n");
	VMProtect::End();
}

void TestProtection()
{
	if (VMProtect::IsProtected(VMProtect::FlagAll))
		printf("protected\n");
	else
		printf("not protected\n");
}

void TestStrings()
{
	VMProtect vmp;
	const char* str1 = VMProtect::String(vmp, "test1");
	const char* str2 = VMProtect::String(&vmp, "test2");
	const char* str3 = VMProtect::String("test3");
	printf("str1: %s\n", str1);
	printf("str2: %s\n", str2);
	printf("str3: %s\n", str3);
	VMProtect::Clean(vmp);
	printf("clean\n");
	printf("str1: %s\n", str1);
	printf("str2: %s\n", str2);
	printf("str3: %s\n", str3);
}

int main()
{
	while (1)
	{
		TestMutation();
		TestProtection();
		TestStrings();
		printf("sleep\n\n");
		Sleep(1000);
	}
	system("pause");
	return 0;
}