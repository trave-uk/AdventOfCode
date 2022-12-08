// day08.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 08/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	int64 part1 = 0;
	int64 part2 = 0;

	std::vector<std::string> grid;

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
				grid.push_back(thisLine);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	for (int y = 0; y < grid.size(); ++y)
	{
		for (int x = 0; x < grid[0].length(); ++x)
		{
			char tree = grid[y][x];
			int hidden = 0;
			int scenic = 1;

			//  left
			int distance = 0;
			for (int dx = x - 1; dx >= 0; --dx)
			{
				++distance;
				if (grid[y][dx] >= tree)
				{
					++hidden;
					break;
				}
			}
			scenic *= distance;

			// right
			distance = 0;
			for (int dx = x + 1; dx < grid[y].length(); ++dx)
			{
				++distance;
				if (grid[y][dx] >= tree)
				{
					++hidden;
					break;
				}
			}
			scenic *= distance;

			// up
			distance = 0;
			for (int dy = y - 1; dy >= 0; --dy)
			{
				++distance;
				if (grid[dy][x] >= tree)
				{
					++hidden;
					break;
				}
			}
			scenic *= distance;

			// down
			distance = 0;
			for (int dy = y + 1; dy < grid.size(); ++dy)
			{
				++distance;
				if (grid[dy][x] >= tree)
				{
					++hidden;
					break;
				}
			}
			scenic *= distance;

			if (hidden < 4)
			{
				++part1;
			}
			if (scenic > part2)
			{
				part2 = scenic;
			}
		}
	}
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 21, 8);
	Process("input.txt");

	return 0;
}
