// day01.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 02/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	int64 part1 = 0;
	int64 part2 = 0;
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	int64 current = 0;
	std::vector<int64> all;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				current += atoi(thisLine);
			}
			else
			{
				if (current > part1)
				{
					part1 = current;
				}
				all.push_back(current);
				current = 0;
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	if (current > part1)
	{
		part1 = current;
	}
	all.push_back(current);

	std::sort(all.begin(), all.end());
	for (int i = all.size() - 1; i >= all.size() - 3; --i)
	{
		part2 += all[i];
	}

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 24000, 45000);
	Process("input.txt");

	return 0;
}
