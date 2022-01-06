// day10.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 6/1/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void Process(std::string input, int part, int times, int64 expectedPart1 = -1)
{
	std::string result = input;
	for (int i = 0; i < times; ++i)
	{
		std::string next;
		char digit = 0;
		int count = 0;
		for (char c : result)
		{
			if (c == digit)
			{
				++count;
			}
			else if (digit)
			{
				char add[20];
				sprintf(add, "%d%c", count, digit);
				next.append(add);
				count = 1;
			}
			else
			{
				count = 1;
			}
			digit = c;
		}
		char add[20];
		sprintf(add, "%d%c", count, digit);
		next.append(add);
		result = next;
	}

	int64 part1 = result.length();
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part %d: %lld\n", input.c_str(), part, part1);
}

int main()
{
	Process("1", 1, 5, 6);
	Process("1113122113", 1, 40);
	Process("1113122113", 2, 50);

	return 0;
}
