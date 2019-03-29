// Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

int main()
{
	int *a, b, c;//定义一指针,两个int
	int *d, *e, *f;//定义三个指针

	class Base
	{
	};

	class Derived : public Base
	{
	};

	Base* bs = new Derived[10];
	delete[] bs;



    std::cout << "Hello World!\n"; 
}

