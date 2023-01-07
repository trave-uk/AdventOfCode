// day23.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 07/01/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

constexpr int OPERATOR(const char* x) { return x[0] * 65536 + x[1] * 256 + x[2]; }

enum Operator
{
	NOOP = 0,
	HLF = OPERATOR("hlf"),
	TPL = OPERATOR("tpl"),
	INC = OPERATOR("inc"),
	JMP = OPERATOR("jmp"),
	JIE = OPERATOR("jie"),
	JIO = OPERATOR("jio")
};

struct Command
{
	Command(const char* o, const char* a1, const char* a2)
		: op(Operator(OPERATOR(o)))
	{
		assert(op == HLF || op == TPL || op == INC || op == JMP || op == JIE || op == JIO);
		args.push_back(a1);
		if (a2)
		{
			args.push_back(a2);
		}
	}
	Operator op = NOOP;
	std::vector<std::string> args;
};

using Program = std::vector<Command>;

int64& Reg(const std::string reg, int64& a, int64& b)
{
	if (reg == "a") return a;
	if (reg == "b") return b;
	assert(false);
}

int64 Run(const Program& p, int64 initialA = 0)
{
	int64 a = initialA;
	int64 b = 0;
	int64 pc = 0;
	do
	{
		const Command& c = p[pc];
		switch (c.op)
		{
		case HLF:
			Reg(c.args[0], a, b) /= 2;
			++pc;
			break;
		case TPL:
			Reg(c.args[0], a, b) *= 3;
			++pc;
			break;
		case INC:
			Reg(c.args[0], a, b) += 1;
			++pc;
			break;
		case JMP:
			pc += atoi(c.args[0].c_str());
			break;
		case JIE:
			if ((Reg(c.args[0], a, b) % 2) == 0)
			{
				pc += atoi(c.args[1].c_str());
			}
			else
			{
				++pc;
			}
			break;
		case JIO:
			if (Reg(c.args[0], a, b) == 1)
			{
				pc += atoi(c.args[1].c_str());
			}
			else
			{
				++pc;
			}
			break;
		default:
			assert(false);
		}
	} while (pc < p.size());
	return b;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	Program program;
	double start = GetMilliseconds();
	int64 part1 = 0;
	int64 part2 = 0;
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				char* op = strtok(thisLine, " ,");
				char* arg1 = strtok(nullptr, " ,");
				char* arg2 = strtok(nullptr, " ,");
				Command cmd(op, arg1, arg2);
				program.push_back(cmd);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	part1 = Run(program);
	if (expectedPart1 != -1)
	{
		printf("[%.2f] %s: Part 1 expected: %lld\n", GetMilliseconds() - start, filename, expectedPart1);
	}
	printf("[%.2f] %s: Part 1: %lld\n", GetMilliseconds() - start, filename, part1);
	assert(expectedPart1 == -1 || expectedPart1 == part1);

	part2 = Run(program, 1);
	if (expectedPart2 != -1)
	{
		printf("[%.2f] %s: Part 2 expected: %lld\n", GetMilliseconds() - start, filename, expectedPart2);
	}
	printf("[%.2f] %s: Part 2: %lld\n", GetMilliseconds() - start, filename, part2);
	assert(expectedPart2 == -1 || expectedPart2 == part2);
}

int main()
{
	Process("example.txt", 0);
	Process("input.txt");

	return 0;
}
