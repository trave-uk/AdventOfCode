// day03.cpp : Advent of Code 2025 https://adventofcode.com/2025
// Solution by trave.uk 03/12/2025 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

int64 CalculateJoltage(const std::string& line, int digits)
{
	int64 joltage = 0;
	int64 prevIndex = -1;
	for (int digit = 1; digit <= digits; ++digit)
	{
		auto line1 = line.substr(0, line.length() + digit - digits);
		int64 index = 0;
		int64 thisMax = 0;
		int64 thisIndex = 0;
		for (char c : line1)
		{
			if (index > prevIndex)
			{
				if (c > thisMax)
				{
					thisMax = c;
					thisIndex = index;
				}
			}
			++index;
		}
		joltage *= 10;
		joltage += thisMax - '0';
		prevIndex = thisIndex;
	}
	return joltage;
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
				std::string line(thisLine);
				part1 += CalculateJoltage(line, 2);
				part2 += CalculateJoltage(line, 12);
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
	Process("example.txt", 357, 3121910778619);
	Process("input.txt");

	return 0;
}
