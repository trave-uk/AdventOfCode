// day04.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 04/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Wordsearch = std::vector<std::string>;

bool CheckWord(std::string& word, const Wordsearch& grid, const coord& pos, const coord& dir)
{
	int64 height = grid.size();
	int64 width = grid[0].length();
	coord p(pos);
	for (char c : word)
	{
		if (p.first < 0 || p.second < 0 || p.first >= width || p.second >= height)
			return false;
		if (grid[p.second][p.first] != c)
			return false;
		p += dir;
	}
	return true;
}

int CountXmas(const Wordsearch& grid)
{
	int64 height = grid.size();
	int64 width = grid[0].length();
	int64 count = 0;

	std::string search = "XMAS";
	for (int64 y = 0; y < width; ++y)
	{
		for (int64 x = 0; x < height; ++x)
		{
			if (grid[y][x] == search[0])
			{
				coord pos(x, y);
				for (int64 dx = -1; dx <= 1; ++dx)
				{
					for (int64 dy = -1; dy <= 1; ++dy)
					{
						coord dir(dx, dy);
						if (dx || dy)
						{
							if (CheckWord(search, grid, pos, dir))
								++count;
						}
					}
				}
			}
		}
	}
	return count;
}

int CountCrossMas(const Wordsearch& grid)
{
	int64 height = grid.size();
	int64 width = grid[0].length();
	int64 count = 0;

	std::string search1 = "MAS";
	std::string search2 = "SAM";
	for (int64 y = 1; y < width - 1; ++y)
	{
		for (int64 x = 1; x < height - 1; ++x)
		{
			if (grid[y][x] == search1[1])
			{
				coord pos(x, y);
				coord dir1(1, 1);
				coord dir2(1, -1);
				coord pos1 = pos;
				pos1 -= dir1;
				coord pos2 = pos;
				pos2 -= dir2;

				if (CheckWord(search1, grid, pos1, dir1) || CheckWord(search2, grid, pos1, dir1))
				{
					if (CheckWord(search1, grid, pos2, dir2) || CheckWord(search2, grid, pos2, dir2))
						++count;
				}
			}
		}
	}
	return count;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	Wordsearch grid;
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
				grid.push_back(line);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	part1 = CountXmas(grid);
	part2 = CountCrossMas(grid);

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);

}

int main()
{
	Process("example.txt", 18, 9);
	Process("input.txt");

	return 0;
}
