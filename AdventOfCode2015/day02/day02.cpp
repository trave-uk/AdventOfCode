// day02.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 31/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	int part1 = 0;
	int part2 = 0;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				int x, y, z;
				int count = sscanf(thisLine, "%dx%dx%d", &x, &y, &z);
				assert(count == 3);
				int xy = x * y;
				int xz = x * z;
				int yz = y * z;
				int mn = min(min(xy, xz), yz);
				part1 += 2*xy + 2*xz + 2*yz + mn;

				int min1 = min(min(x, y), z);
				int min2 = (min1 == x) ? min(y, z) : ((min1 == y) ? min(x, z) : min(x, y));
				part2 += (2 * (min1 + min2)) + (x * y * z);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %d\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %d\n", filename, part2);
}

int main()
{
	Process("example.txt", 58+43, 34+14);
	Process("input.txt");

	return 0;
}
