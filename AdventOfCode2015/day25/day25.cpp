// day25.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 08/01/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

int64 Search(int64 row, int64 column)
{
	int64 x = 1, y = 1;
	int64 result = 20151125;
	while (x != column || y != row)
	{
		x++;
		y--;
		if (y == 0)
		{
			y = x;
			x = 1;
		}

		result = (result * 252533) % 33554393;
	}
	return result;
}

void Process(const char* filename, int64 expectedPart1 = -1)
{
	double start = GetMilliseconds();
	int64 row = 0, column = 0;
	int64 part1 = 0;
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
				// To continue, please consult the code grid in the manual.  Enter the code at row 2947, column 3029.
				int m = sscanf(thisLine, "To continue, please consult the code grid in the manual.  Enter the code at row %lld, column %lld.", &row, &column);
				assert(m == 2);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	part1 = Search(row, column);

	if (expectedPart1 != -1)
	{
		printf("[%.2f] %s: Part 1 expected: %lld\n", GetMilliseconds() - start, filename, expectedPart1);
	}
	printf("[%.2f] %s: Part 1: %lld\n", GetMilliseconds() - start, filename, part1);
	assert(expectedPart1 == -1 || expectedPart1 == part1);
}

int main()
{
	Process("example.txt", 9250759);
	Process("input.txt");

	return 0;
}
