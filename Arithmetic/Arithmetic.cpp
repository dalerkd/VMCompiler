// 四则分析依据轮子哥.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <string>
#include <cassert>

#include "debug.h"
#include  "EXCEP.h"



/*
 * 根据以下三条写成.
1)       Term=<数字> | “(“ Exp “)”

2)       Factor = Term ( ( “*” | “/” ) Term)*

3)       Exp = Factor ( ( “+” | “-“ ) Factor)*

产生式:
http://www.cppblog.com/vczh/archive/2008/06/15/53373.html
 *
 *
 */


bool g_OPEN_LOG = true;

class Four_Oper_Analy
{
public:
	void displayASM(bool b)
	{
		m_output_asm = b;
	}
	void displayLex(bool b)
	{
		m_output_lex = b;
	}
private:
	class DECIDE_END
	{

	};

	enum type
	{
		E_UNVALID = 0,
		E_NUMBER = 1,
		E_OPER = 2,
		E_END = 3
	};

	struct stru
	{
		stru() {
			type = E_UNVALID;
			number = 0;
			oper = 'Y';
		}
		type type;
		int number;
		char oper;
	};

public:
	void setBuffer(std::string buffer)
	{
		m_buffer = buffer;
		m_exp_now = const_cast<char*>(m_buffer.c_str());
		next();
	}
private:
	bool m_output_asm = false;
	bool m_output_lex = true;
	char* m_exp_now = nullptr;
	std::string m_buffer;
	stru m_stru_;
	stru get_token()
	{
		return m_stru_;
	}
	void set_token(stru t)
	{
		m_stru_ = t;
		if (m_output_lex)
		{
			printf("set type: %s\r\n", TokenTypeConvert2String(t).c_str());
		}
	}
	/*
	 * 获取下一个token
	 *
	 * Exception:
	 * 1. DECIDE_END 终止符
	 * 2. char* : 语法错误等
	 *
	 */

	void next()
	{
		if (m_buffer.empty())
		{
			OutputDebug(L"");
			OutputDebugStringA("1234");
			LogError("m_buffer is empty");
			;
		}

		stru stru_save;
		// add safe limit 要判断当前的操作是不是无效
		while (true) {
			{
				int all_size = m_buffer.size();

				int c = m_exp_now - m_buffer.c_str() + 1;
				if (c > all_size)
				{
					if (stru_save.type == E_NUMBER)
					{
						set_token(stru_save);
						return;
					}
					else
					{
						stru_save.type = E_END;
						set_token(stru_save);
						throw(DECIDE_END());
					}
				}
			}
			char tmp = *m_exp_now;
			switch (tmp) {
			case '0':case'1':case'2':case'3':case'4':case'5':case'6':case'7':case'8':case'9':
			{
				if (stru_save.type != E_NUMBER && stru_save.type != E_UNVALID)
				{
					m_exp_now--;
					set_token(stru_save);
					return;
				}

				stru_save.type = E_NUMBER;
				stru_save.number = stru_save.number * 10 + (tmp - '0');

				break;
			}
			case '+':case'-':case'*':case'/':case'(':case')':
			{
				if (stru_save.type != E_UNVALID)
				{
					set_token(stru_save);
					return;
				}
				stru_save.type = E_OPER;
				stru_save.oper = tmp;
				set_token(stru_save);
				m_exp_now++;
				return;
				break;
			}
			case ' ':case'\t':
			{
				break;
			}
			default:
			{
				LogError("未预期的字符:%c(%d)\r\n", tmp, tmp);
			}
			}
			m_exp_now++;
		}
	}

	void IsEnd()
	{
		if (get_token().type == E_END)
		{
			throw(DECIDE_END());
		}
	}
	std::string TokenTypeConvert2String(stru ty)
	{

		char buffer[256] = { 0 };

		switch (ty.type)
		{
		case E_UNVALID:
			sprintf_s(buffer, "UNVALID\r\n");
			break;
		case E_NUMBER:
			sprintf_s(buffer, "NUMBER, Value:%d", ty.number);
			break;
		case E_OPER:
			sprintf_s(buffer, "OPER, Value:%c", ty.oper);
			break;
		case E_END:
			sprintf_s(buffer, "END");
			break;
		default: {
			sprintf_s(buffer, "未知类型");
			break;
		}
		}
		return buffer;
	}
	void mustIsOper()
	{
		if (get_token().type != E_OPER)
		{
			LogError("期待标识符为OPER,结果是:%s", TokenTypeConvert2String(get_token()).c_str());
		}
	}
	void mustIsNumber()
	{
		if (get_token().type != E_NUMBER)
		{
			LogError("期待标识符为NUMBER,结果是:%s", TokenTypeConvert2String(get_token()).c_str());
		}
	}
	//不是操作符会异常
	char get_oper()
	{
		mustIsOper();
		return get_token().oper;
	}
	//不是数字会异常
	int get_number()
	{
		mustIsNumber();
		return get_token().number;
	}

	/*
	 *

	Term = <数字> | "(" Exp ")"
	Factor = Term (("*"|"/")Term)*
	Exp = Factor (("+"|"-")Factor)*

		*/
	int parseTerm()
	{
		int number;
		try {
			number = get_number();
			next();
			if (m_output_asm)
			{
				printf("push  %d\r\n", number);
			}
			return number;
		}
		catch (DECIDE_END&)
		{
			if (m_output_asm)
			{
				printf("push  %d\r\n", number);
			}
			return number;
		}
		catch (...)
		{
			;
		}

		char L = get_oper();
		if (L != '(')
		{
			LogError("期待'(',结果是:'%c'ASCII:[%d]", L, L);
		}
		next();

		int result = parseExp();

		char R = get_oper();
		if (R != ')')
		{
			LogError("期待')',结果是:'%c'ASCII:[%d]", R, R);
		}
		try {
			next();
		}
		catch (DECIDE_END&)
		{
		}
		if (m_output_asm)
		{
			printf("push  %d\r\n", result);
		}
		return result;


	}
	int parseExp()
	{
		int tmp = parseFactor();
		/*
		 * 怎么表达这种可以不存在的东西?
		 * 1. 抛一个对象出来,当没有任何东西时
		 * 2. match_end 匹配空
		 *
		 */
		while (true)
		{
			char op = 'Y';
			try {
				IsEnd();
				op = get_oper();
			}
			catch (DECIDE_END&)
			{
				return tmp;
			}
			if (op != '+'&&op != '-')
			{
				break;
			}
			next();
			// 继续获取下一个 Factor
			int secondResult = parseFactor();

			if (op == '+')
			{
				tmp += secondResult;
				if (m_output_asm)
				{
					printf("pop   ebx\r\n");
					printf("pop   eax\r\n");
					printf("add   eax,ebx\r\n");
					printf("push  eax\r\n");
				}

			}
			else if (op == '-')
			{
				tmp -= secondResult;
				if (m_output_asm)
				{
					printf("pop   ebx\r\n");
					printf("pop   eax\r\n");
					printf("sub   eax,ebx\r\n");
					printf("push  eax\r\n");
				}

			}
			else
			{
				LogError("期待得到+-,结果得到:%c(%d)", op, op);
			}
		}///-end-while
		return tmp;

	}


	int parseFactor()
	{
		int tmp = parseTerm();
		while (true)
		{
			char op = 'Y';
			try {
				IsEnd();
				op = get_oper();
			}
			catch (DECIDE_END&)
			{
				return tmp;
			}


			if (op != '*'&&op != '/')
			{
				break;
			}
			next();

			// 继续获取下一个 Term
			int secondResult = parseTerm();

			if (op == '*')
			{
				tmp *= secondResult;
				if (m_output_asm)
				{
					/* 无符号乘法
					 * 如果源操作数是32位的，则与EAX相乘，积存储在EDX:EAX中。
					 */
					printf("pop   ebx\r\n");
					printf("pop   eax\r\n");
					printf("mul   ebx\r\n");
					printf("push  eax\r\n");
				}

			}
			else if (op == '/')
			{
				if (secondResult == 0)
				{
					LogError("尝试除0");
				}
				tmp /= secondResult;
				if (m_output_asm)
				{
					/* https://blog.csdn.net/jiangwei0512/article/details/50611604
					 * 无符号整数除法:
					 * 如果除数是32位的，被除数是EDX:EAX，商在EAX中，余数在EDX中。
					 */
					printf("pop   eax\r\n"); // 除数
					printf("mov   edx,0\r\n");
					printf("pop   ebx\r\n"); // 被除数
					printf("div   ebx\r\n");
					printf("push  eax\r\n");
				}

			}
			else
			{
				LogError("期待得到+-,结果得到:%c(%d)", op, op);
			}



		}///-end-while
		return tmp;
	}

public:
	/*入口*/
	int calc()
	{
		try {

			int result = parseExp();
			if (m_output_asm)
			{
				printf("pop   eax\r\n");
			}
			return result;
		}
		catch (DECIDE_END&)
		{
			printf("遭遇意料外的终止\r\n");
		}
		catch (EXCEP& e)
		{
			printf("Exception:%s  ", e.data);
			printf("发生在offset: %d\r\n", m_exp_now - m_buffer.c_str());
		}
		return -1;
	}
	static void testSelf()
	{
		{
			Four_Oper_Analy foa;
			std::string str = "1+2*3";
			printf("%s\r\n", str.c_str());
			foa.displayASM(true);
			foa.displayLex(false);
			foa.setBuffer(str);
			assert(7 == foa.calc());
			printf("测试一结束\r\n\r\n");
		}
		{
			Four_Oper_Analy foa;
			std::string str = "10+2*3-(20+20)";
			printf("%s\r\n", str.c_str());
			foa.displayASM(true);
			foa.displayLex(false);
			foa.setBuffer(str);
			assert(-24 == foa.calc());
			printf("测试二结束\r\n\r\n");
		}
		printf("测试通过 :)\r\n");
	}
};

int main()
{
	Four_Oper_Analy test;
	test.testSelf();

	{
		Four_Oper_Analy foa;
		std::string buffer;
		printf("请输入表达式:即时计算:\r\n");
		foa.displayASM(true);
		foa.displayLex(false);
		while ((std::getline(std::cin, buffer))) {
			//
			
			foa.setBuffer(buffer);
			int m_number;

			m_number = foa.calc();

			printf("结果是:%d\n", m_number);
		}
	}

}

