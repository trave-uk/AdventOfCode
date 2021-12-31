// day04.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 31/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void Process(std::string input, int64 expectedPart1 = -1)
{
	MD5 md5;
	int64 part1 = -1;
	int64 index = 0;
	for (;;)
	{
		std::string test = input + std::to_string(index);
		char* digest = md5.digestString(test.c_str());
		if (part1 == -1 && strncmp(digest, "00000", 5) == 0)
		{
			part1 = index;
		}
		if (strncmp(digest, "000000", 6) == 0)
		{
			break;
		}
		++index;
	}
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", input.c_str(), part1);
	printf("%s: Part 2: %lld\n", input.c_str(), index);
}

int main()
{
	Process("abcdef", 609043);
	Process("pqrstuv", 1048970);
	Process("ckczppom");

	return 0;
}
