// day18.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

enum class OP
{
	ADD,
	MUL,
	LEFTBRACKET,
};

enum TYPE
{
	TYPE_OPERATOR,
	TYPE_VALUE
};

struct StackEntry
{
	StackEntry(__int64 value)
	{
		type = TYPE_VALUE;
		op = OP::LEFTBRACKET;
		val = value;
	}
	StackEntry(OP oper)
	{
		type = TYPE_OPERATOR;
		op = oper;
		val = 0;
	}

	TYPE type;
	OP op;
	__int64 val;
};

__int64 evaluate(char* expression, int part)
{
	// Use shunting-yard algorithm to generate commands
	// https://en.wikipedia.org/wiki/Shunting-yard_algorithm

	std::vector<StackEntry> postFixQueue;
	std::vector<OP> operatorStack;
	char* token = expression;
	while (*token)
	{
		if (isdigit(*token) || (*token == '-' && isdigit(*(token + 1))))
		{
			postFixQueue.push_back(atoll(token));
			token += strspn(token, "0123456789");
		}
		else
		{
			char tok = *token;
			++token;
			if (tok == '(')
			{
				operatorStack.push_back(OP::LEFTBRACKET);
			}
			else if (tok == ')')
			{
				while (!operatorStack.empty())
				{
					OP op = operatorStack.back();
					if (op != OP::LEFTBRACKET)
					{
						operatorStack.pop_back();
						postFixQueue.push_back(op);
					}
					else
					{
						break;
					}
				}
				operatorStack.pop_back(); // OP::LEFTBRACKET
			}
			else if (tok != ' ')
			{
				while (!operatorStack.empty())
				{
					OP op = operatorStack.back();
					if (part == 2 && op == OP::MUL && tok == '+')
					{
						// addition higher precidence than multiplication in part 2
						break;
					}
					else if (op != OP::LEFTBRACKET)
					{
						operatorStack.pop_back();
						postFixQueue.push_back(op);
					}
					else
					{
						break;
					}
				}
				switch (tok)
				{
				case ')':
					break;
				case '+':
					operatorStack.push_back(OP::ADD);
					break;
				case '*':
					operatorStack.push_back(OP::MUL);
					break;
				default:
					assert(false);
					break;
				}
			}
		}
	}
	for (auto iter = operatorStack.rbegin(); iter != operatorStack.rend(); ++iter)
	{
		postFixQueue.push_back(*iter);
	}

	// Now evaluate
	std::vector<__int64> queue;
	for (StackEntry e : postFixQueue)
	{
		if (e.type == TYPE_VALUE)
		{
			queue.push_back(e.val);
		}
		else
		{
			assert(e.type == TYPE_OPERATOR);
			__int64 second = queue.back();
			queue.pop_back();
			__int64 first = queue.back();
			queue.pop_back();
			__int64 result = 0;
			switch (e.op)
			{
			case OP::ADD:
				result = first + second;
				break;
			case OP::MUL:
				result = first * second;
				break;
			default:
				assert(false);
				break;
			}
			queue.push_back(result);
		}
	}
	assert(queue.size() == 1);
	return queue[0];
}

int main()
{
	for (int part = 1; part <= 2; ++part)
	{
		__int64 total = 0;
		char* buffer = new char[65536];
		//FILE *fp = fopen("example.txt", "rt");
		FILE *fp = fopen( "input.txt", "rt" );
		while (!feof(fp))
		{
			char* thisLine = fgets(buffer, 65536, fp);
			if (thisLine)
			{
				thisLine[strcspn(thisLine, "\n\r")] = '\0';
				__int64 value = evaluate(thisLine, part);
				total += value;
				//printf("%s becomes %lld.\n", thisLine, value);
			}
		}
		fclose(fp);
		delete[] buffer;

		printf("Part %d: %lld\n", part, total);
	}
	return 0;
}

// 5554483793 too low
