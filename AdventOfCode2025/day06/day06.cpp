// day06.cpp : Advent of Code 2025 https://adventofcode.com/2025
// Solution by trave.uk 06/12/2025 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Operands = std::vector<int64>;
enum Operator
{
	ADD = '+',
	MULTIPLY = '*'
};
using Operators = std::vector<Operator>;

int64 calculateSum(const Operators& operators, const std::vector<Operands>& allOperands)
{
	int64 total = 0;
	size_t count = operators.size();
	for (size_t i = 0; i < count; ++i)
	{
		Operator op = operators[i];
		int64 result = (op == ADD) ? 0 : 1;
		for (auto& operands : allOperands)
		{
			if (op == ADD)
				result += operands[i];
			else
				result *= operands[i];
		}
		total += result;
	}
	return total;
}

int64 calculateSum2(const Operators& operators, const std::vector<Operands>& allOperands)
{
	int64 total = 0;
	size_t count = operators.size();
	for (size_t i = 0; i < count; ++i)
	{
		Operator op = operators[i];
		int64 result = (op == ADD) ? 0 : 1;
		for (auto& operand : allOperands[i])
		{
			if (op == ADD)
				result += operand;
			else
				result *= operand;
		}
		total += result;
	}
	return total;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	std::vector<Operands> allOperands;
	Operators allOperators;
	std::vector<std::string> operandLines;
	std::string operatorLine;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			bool isOperand = isdigit(*thisLine) || isspace(*thisLine);
			std::string line(thisLine);
			if (isOperand)
				operandLines.push_back(line);
			else
				operatorLine = line;
			if (*thisLine)
			{
				Operands operands;
				Operators operators;
				auto token = strtok(thisLine, " ");
				while (token)
				{
					if (isOperand)
					{
						operands.push_back(atoll(token));
					}
					else
					{
						operators.push_back(Operator(*token));
					}
					token = strtok(nullptr, " ");
				}
				if (isOperand)
				{
					allOperands.push_back(operands);
				}
				else
				{
					assert(allOperators.empty());
					allOperators = operators;
				}
			}

		}
	}
	fclose(fp);
	delete[] buffer;

	part1 = calculateSum(allOperators, allOperands);

	// Part 2: extract operands from the lines vertically...
	int index = 0;
	int opIndex = 0;
	std::vector<Operands> part2Operands;
	Operands currentOperands;
	for (char op : operatorLine)
	{
		int64 num = 0;
		bool found = false;
		for (auto line : operandLines)
		{
			if (line[index] == ' ')
				continue;
			found = true;
			num *= 10;
			num += line[index] - '0';
		}
		if (found)
		{
			currentOperands.push_back(num);
		}
		else
		{
			part2Operands.push_back(currentOperands);
			currentOperands.erase(currentOperands.begin(), currentOperands.end());
		}
		++index;
	}
	if (!currentOperands.empty())
		part2Operands.push_back(currentOperands);

	part2 = calculateSum2(allOperators, part2Operands);

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 4277556, 3263827);
	Process("input.txt");

	return 0;
}
