// day04.cpp : Advent of Code 2025 https://adventofcode.com/2025
// Solution by trave.uk 04/12/2025 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Map = std::vector<std::string>;

bool HasRoll(const Map& map, int x, int y)
{
	const int width = map[0].length();
	const int height = map.size();

	if (x < 0 || y < 0 || x >= width || y >= height)
		return false;
	return map[y][x] == '@';
}

int CountNeighbours(const Map& map, int x, int y)
{
	int count = 0;
	for (int dy = -1; dy <= 1; ++dy)
	{
		for (int dx = -1; dx <= 1; ++dx)
		{
			if (!dx && !dy)
				continue;
			if (HasRoll(map, x + dx, y + dy))
			{
				count++;
			}
		}
	}
	return count;
}

int CalcPart1(const Map& map)
{
	int64 part1 = 0;
	int width = map[0].length();
	int height = map.size();
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < height; ++x)
		{
			if (map[y][x] == '@')
			{
				int neighbours = CountNeighbours(map, x, y);
				if (neighbours < 4)
				{
					++part1;
				}
			}
		}
	}
	return part1;
}

int CalcPart2(const Map& map)
{
	Map _map = map;
	int64 part2 = 0;
	int width = map[0].length();
	int height = map.size();
	while (true)
	{
		Map newMap = _map;
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < height; ++x)
			{
				if (_map[y][x] == '@')
				{
					int neighbours = CountNeighbours(_map, x, y);
					if (neighbours < 4)
					{
						++part2;
						newMap[y][x] = '.';
					}
				}
			}
		}
		if (newMap == _map)
			break;
		_map = newMap;
	}
	return part2;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	Map map;
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
				map.push_back(line);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	part1 = CalcPart1(map);
	part2 = CalcPart2(map);

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 13, 43);
	Process("input.txt");

	return 0;
}
