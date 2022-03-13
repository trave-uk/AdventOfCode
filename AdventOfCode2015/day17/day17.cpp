// day17.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 13/03/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void Process(const char* filename, int64 target, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	std::vector<int> containers;
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
				containers.push_back(atoi(thisLine));
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	int permutations = 1 << containers.size();
	int least = containers.size();
	for (int i = 0; i < permutations; ++i)
	{
		int total = 0;
		int index = 0;
		int test = i;
		int count = 0;
		while (test && total < target)
		{
			if (test % 2)
			{
				total += containers[index];
				++count;
				if (total == target && test == 1)
				{
					++part1;
					if (count < least)
					{
						part2 = 0;
						least = count;
					}
					if (count == least)
					{
						++part2;
					}
				}
			}
			test /= 2;
			++index;
		}
	}
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 25, 4, 3);
	Process("input.txt", 150);

	return 0;
}
