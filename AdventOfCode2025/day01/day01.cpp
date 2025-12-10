// day01.cpp : Advent of Code 2025 https://adventofcode.com/2025
// Solution by trave.uk 01/12/2025 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	int dial = 50;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				int lastDial = dial;
				int val = atoi(thisLine + 1);
				if (val >= 100)
				{
					part2 += val / 100;
					val = val % 100;
				}
				switch (thisLine[0])
				{
				case 'L':
					dial -= val;
					if (dial < 0)
					{
						if (lastDial > 0)
							++part2;
						dial += 100;
					}
					break;
				case 'R':
					dial += val;
					if (dial > 100)
					{
						++part2;
						dial -= 100;
					}
					if (dial == 100)
						dial = 0;
					break;
				}
				if (dial == 0)
				{
					++part1;
					++part2;
				}
				if (expectedPart2 > 0)
					printf("%s: %d (%lld %lld)\n", thisLine, dial, part1, part2);
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
	Process("example.txt", 3, 6);
	Process("input.txt");

	return 0;
}
