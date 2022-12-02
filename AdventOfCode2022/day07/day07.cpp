// day07.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 07/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
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

			}
		}
	}
	fclose(fp);
	delete[] buffer;

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

//	assert(expectedPart2 == -1 || expectedPart2 == part2);
//	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt");
	Process("input.txt");

	return 0;
}
