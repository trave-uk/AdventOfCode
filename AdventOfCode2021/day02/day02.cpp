// day02.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 2/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void Process(const char* filename)
{
	int forward = 0;
	int depth1 = 0;

	int depth2 = 0;
	int aim2 = 0;
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
				char direction[10];
				int distance;
				int matched = sscanf(thisLine, "%s %d", direction, &distance);
				assert(matched == 2);
				if (strcmp(direction, "forward") == 0)
				{
					forward += distance;
					depth2 += aim2 * distance;
				}
				else if (strcmp(direction, "down") == 0)
				{
					depth1 += distance;
					aim2 += distance;
				}
				else if (strcmp(direction, "up") == 0)
				{
					depth1 -= distance;
					aim2 -= distance;
				}
				else
				{
					assert(false);
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	printf("%s: Part 1: %d * %d = %d\n", filename, forward, depth1, forward * depth1);
	printf("%s: Part 2: %d * %d = %d\n", filename, forward, depth2, forward * depth2);
}

int main()
{
	Process("example.txt");
	Process("input.txt");

	return 0;
}
