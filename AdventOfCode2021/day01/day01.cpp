// day01.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 1/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void Process(const char* filename)
{
	int part1 = 0, part2 = 0;
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	int last = -1;
	int last2 = -1;
	int up3 = 0;
	int back2 = -1;
	int back3 = -1;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				int reading = atoi(thisLine);
				if (last > 0 && reading > last)
				{
					++part1;
				}

				up3 += reading;
				if (back3 >= 0)
				{
					up3 -= back3;
					if (last2 > 0 && up3 > last2)
					{
						++part2;
					}
				}

				last2 = up3;
				back3 = back2;
				back2 = last;
				last = reading;
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	printf("%s: Part 1: %d\n", filename, part1);
	printf("%s: Part 2: %d\n", filename, part2);
}

int main()
{
	Process("example.txt");
	Process("input.txt");

	return 0;
}
