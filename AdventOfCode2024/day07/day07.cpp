// day07.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 07/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

class Equation
{
public:
	int64 result;
	std::deque<int64> numbers;

	int64 pop_front()
	{
		int64 f = numbers.front();
		numbers.pop_front();
		return f;
	}
};

int64 append(const int64& l, const int64& r)
{
	char join[128];
	sprintf_s(join, "%lld%lld", l, r);
	int64 j = _atoi64(join);
	return j;
}

int64 TestEquation(const Equation& e, int64 a = 0, int part = 1)
{
	if (e.numbers.size() == 0)
	{
		if (a == e.result)
			return a;
		else
			return 0;
	}
	auto e1 = e;
	int64 n = e1.pop_front();
	int64 result = TestEquation(e1, a + n, part);
	if (!result)
		result = TestEquation(e1, a * n, part);
	if (!result && part == 2)
		result = TestEquation(e1, append(a, n), part);
	return result;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				int pos = 0;
				Equation e;
				char *tok = strtok(thisLine, ":");
				e.result = _atoi64(tok);
				tok = strtok(nullptr, " ");
				while (tok)
				{
					e.numbers.push_back(_atoi64(tok));
					tok = strtok(nullptr, " ");
				}
				int64 f = e.pop_front();
				int64 valid = TestEquation(e, f);
				if (valid)
					part1 += valid;
				else
					valid = TestEquation(e, f, 2);
				part2 += valid;
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);

}

int main()
{
	Process("example.txt", 3749, 11387);
	Process("input.txt");

	return 0;
}
